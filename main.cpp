#include <iostream>
#include <fstream>
#include <thread>
#include <sys/inotify.h>
#include <unistd.h>
#include "Looper.h"
#include "Handler.h"


#define LOG_ENABLED 1           // Bật log trong file này
//#define LOG_FILE_PATH "/tmp/main_log.log" // Tệp log dành riêng cho file này
#include "Log.h"


using namespace std;

class MainHandler : public Handler {
public:
    MainHandler(int id) : Handler(id) {}
    void handleMessage(std::shared_ptr<Message> msg) {
        //cout<<"msg id "<<msg->what<<" content: "<<msg->content<<"thrs id "<<std::this_thread::get_id()<<endl;
    }
};


void test() {
        int x = 10, y = 20;
        float z = 3.14;
        std::string s = "Hello";

        DEBUG_LOG("HAHA   ",x, y);        // In ra: x[10] y[20]
        DEBUG_LOG(x, z, s);     // In ra: x[10] z[3.14] s[Hello]
        DEBUG_LOG(x + y, z);    // In ra: x + y[30] z[3.14]
}

int main() {
    // const std::string filePath = "test.txt";
    
    std::shared_ptr<MainHandler> mHandler = std::make_shared<MainHandler>(1);
    while(true) {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        auto msg = std::make_shared<Message>(3);
        msg->content = "hello";
        //cout<<"post msg on thrs: "<<std::this_thread::get_id()<<endl;
        mHandler->post(msg);
        test();
        
    }
    mHandler->join();
    


}
