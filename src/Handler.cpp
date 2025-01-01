#include <memory>
#include "MessageQueue.h"
#include "Looper.h"
#include "Handler.h"

#define LOG_ENABLED 1           // Bật log trong file này
//#define LOG_FILE_PATH "/tmp/handler.log" // Tệp log dành riêng cho file này
#include "Log.h"

Handler::LooperCallback::LooperCallback(Handler& _handler) : mparent(_handler) {}

void Handler::LooperCallback::SendtoHandler(std::shared_ptr<Message> msg) {
    mparent.handleMessage(msg);
}

Handler::Handler(int id) : mid(id), messageQueue(std::make_shared<MessageQueue>()) {
    mloopercb = std::make_shared<LooperCallback>(*this);
    mlooper = std::make_shared<Looper>(messageQueue, mloopercb);
    start();
}

void Handler::start() {
    looperThread = std::thread([&]() {
        mlooper->loop();
    });
}
void Handler::join() {
    looperThread.join();
}

void Handler::post(const std::shared_ptr<Message>& message) {
    DEBUG_LOG("Post log ", message->content);
    messageQueue->enqueueMessage(message);
}

