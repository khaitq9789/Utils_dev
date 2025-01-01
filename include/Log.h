#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <memory>
#include <cstring>

// Mỗi file có thể tự định nghĩa LOG_ENABLED và LOG_FILE_PATH
#ifndef LOG_ENABLED
#define LOG_ENABLED 0  // 0: Tắt log, 1: Bật log
#endif

#ifndef LOG_FILE_PATH
#define LOG_FILE_PATH "/tmp/default.log"
#endif


// Hàm xử lý giá trị trực tiếp (không phải tên biến)
template <typename T>
std::string format_direct_entry(const T& value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

// Hàm xử lý tên và giá trị của một tham số
template <typename T>
std::string format_log_entry(const char* name, const T& value) {
    std::ostringstream oss;
    oss << "(" << name << ") [" << value << "]";
    return oss.str();
}

// Trường hợp cơ sở (base case): chỉ còn một tham số, và đây là chuỗi trực tiếp
template <typename T>
void stringify_helper(std::ostringstream& oss, const T& value) {
    oss << format_direct_entry(value);  // Không cần xử lý tên biến
}

// Trường hợp cơ sở (base case): chỉ còn một tham số, với tên và giá trị
template <typename T>
void stringify_helper(std::ostringstream& oss, const char* name, const T& value) {
    oss << format_log_entry(name, value);
}

// Hàm đệ quy để nối tất cả các tham số thành chuỗi
template <typename T, typename... Args>
void stringify_helper(std::ostringstream& oss, const char* names, const T& first, const Args&... rest) {
    // Tìm vị trí dấu phẩy
    const char* comma_pos = strchr(names, ',');

    if (comma_pos) {
        // Xử lý tên tham số hiện tại
        std::string current_name(names, comma_pos - names);
        while (!current_name.empty() && current_name.back() == ' ') {
            current_name.pop_back();  // Loại bỏ khoảng trắng thừa
        }

        // Nếu đây là một chuỗi trực tiếp (không phải tên biến)
        if (current_name.empty() || current_name.find(' ') != std::string::npos) {
            oss << format_direct_entry(first) << " ";
        } else {
            // Thêm entry log cho tham số hiện tại
            oss << format_log_entry(current_name.c_str(), first) << " ";
        }

        // Đệ quy cho các tham số còn lại
        stringify_helper(oss, comma_pos + 1, rest...);
    } else {
        // Xử lý tham số cuối cùng
        stringify_helper(oss, names, first);
    }
}

// Hàm bên ngoài để khởi tạo quá trình nối tham số
template <typename... Args>
std::string stringify(const char* names, const Args&... args) {
    std::ostringstream oss;
    stringify_helper(oss, names, args...);  // Bắt đầu đệ quy
    return oss.str();
}

// Macro để ghi log
#if LOG_ENABLED
#define DEBUG_LOG(...)                                  \
    do {                                                \
        std::string log_msg = stringify(#__VA_ARGS__, __VA_ARGS__); \
        std::cout << "[" << __FILE__ << "::" << __func__ << "] " << log_msg << std::endl; \
        static std::shared_ptr<std::ofstream> log_file_ptr = nullptr;  \
        if (!log_file_ptr) {                            \
            log_file_ptr = std::make_shared<std::ofstream>(LOG_FILE_PATH, std::ios::app); \
            if (!log_file_ptr->is_open()) {             \
                std::cerr << "Không thể mở tệp log tại: " << LOG_FILE_PATH << std::endl;  \
                throw std::runtime_error("Lỗi mở tệp log."); \
            }                                           \
        }                                               \
        if (log_file_ptr && log_file_ptr->is_open()) {  \
            *log_file_ptr << "[" << __FILE__ << "::" << __func__ << "] " << log_msg << std::endl; \
        }                                               \
    } while (0)
#else
#define DEBUG_LOG(...) do {} while (0)
#endif

#endif // LOG_H
