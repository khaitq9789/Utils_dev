#include <iostream>
#include <fstream>
#include <thread>
#include <sys/inotify.h>
#include <unistd.h>
#include "Looper.h"
#include "Handler.h"

using namespace std;

class MainHandler : public Handler {
public:
    MainHandler(int id) : Handler(id) {}
    void handleMessage(std::shared_ptr<Message> msg) {
        cout<<"msg id "<<msg->what<<" content: "<<msg->content<<"thrs id "<<std::this_thread::get_id()<<endl;
    }
};



int main() {
    const std::string filePath = "test.txt";


    std::shared_ptr<MainHandler> mHandler = std::make_shared<MainHandler>(1);
    while(true) {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        auto msg = std::make_shared<Message>(3);
        msg->content = "hello";
        cout<<"post msg on thrs: "<<std::this_thread::get_id()<<endl;
        mHandler->post(msg);
    }
    mHandler->join();

    std::cout << "Program exited.\n";
    return 0;
}
