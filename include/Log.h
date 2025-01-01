#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <cstring>

// Định nghĩa mặc định nếu không được đặt trong file .cpp
#ifndef LOG_ENABLED
#define LOG_ENABLED 0  // 0: Tắt log, 1: Bật log
#endif

#ifndef LOG_FILE_PATH
#define LOG_FILE_PATH "/tmp/default.log"
#endif

#ifndef LOG_TO_CONSOLE
#define LOG_TO_CONSOLE 1  // 1: Bật log ra console, 0: Tắt
#endif

#if LOG_ENABLED
inline std::shared_ptr<std::ofstream>& get_log_file_ptr() {
    static std::shared_ptr<std::ofstream> log_file_ptr = nullptr;
    if (!log_file_ptr) {
        log_file_ptr = std::make_shared<std::ofstream>(LOG_FILE_PATH, std::ios::app);
        if (!log_file_ptr->is_open()) {
            std::cerr << "Không thể mở tệp log tại: " << LOG_FILE_PATH << std::endl;
        }
    }
    return log_file_ptr;
}
#endif

// Hàm xử lý dấu cách thừa
inline std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(' ');
    size_t end = str.find_last_not_of(' ');
    return (start == std::string::npos || end == std::string::npos) ? "" : str.substr(start, end - start + 1);
}

// Hàm hỗ trợ định dạng log
template <typename T>
std::string format_log_entry(const char* name, const T& value) {
    std::ostringstream oss;
    oss << "(" << name << ") [" << value << "]";
    return oss.str();
}

template <typename T>
std::string format_direct_entry(const T& value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

// Trường hợp cơ sở cho tham số cuối cùng
template <typename T>
void stringify_helper(std::ostringstream& oss, const char* names, const T& value) {
    std::string trimmed_name = trim(names);  // Loại bỏ dấu cách thừa
    oss << format_log_entry(trimmed_name.c_str(), value);
}

// Hàm đệ quy để phân tách và xử lý các tham số
template <typename T, typename... Args>
void stringify_helper(std::ostringstream& oss, const char* names, const T& first, const Args&... rest) {
    const char* comma_pos = strchr(names, ',');
    if (comma_pos) {
        // Tách tên biến hiện tại và xử lý dấu cách
        std::string current_name(names, comma_pos - names);
        current_name = trim(current_name);  // Loại bỏ dấu cách thừa

        if (!current_name.empty() && current_name.find(' ') == std::string::npos) {
            // In tên biến và giá trị
            oss << format_log_entry(current_name.c_str(), first) << " ";
        } else {
            // Giá trị trực tiếp
            oss << format_direct_entry(first) << " ";
        }

        stringify_helper(oss, comma_pos + 1, rest...);
    } else {
        // Xử lý tham số cuối cùng
        stringify_helper(oss, names, first); // Gọi hàm cơ sở khi chỉ còn một tham số
    }
}

// Hàm chính để nối các tham số thành chuỗi
template <typename... Args>
std::string stringify(const char* names, const Args&... args) {
    std::ostringstream oss;
    stringify_helper(oss, names, args...);
    return oss.str();
}

// Macro để ghi log
#if LOG_ENABLED
#define DEBUG_LOG(...)                                  \
    do {                                                \
        std::string log_msg = stringify(#__VA_ARGS__, __VA_ARGS__); \
        if (LOG_TO_CONSOLE) {                           \
            std::cout << "[" << __FILE__ << "::" << __func__ << "] " << log_msg << std::endl; \
        }                                               \
        auto& log_file_ptr = get_log_file_ptr();        \
        if (log_file_ptr && log_file_ptr->is_open()) {  \
            *log_file_ptr << "[" << __FILE__ << "::" << __func__ << "] " << log_msg << std::endl; \
        }                                               \
    } while (0)
#else
#define DEBUG_LOG(...) do {} while (0)
#endif

#endif // LOG_H
