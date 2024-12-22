#ifndef HANDLER_H
#define HANDLER_H

#include <memory>
#include "MessageQueue.h"
#include "Looper.h"
#include <thread>

class Handler {
private:
    class LooperCallback : public ILooperCallback {
    public:
        explicit LooperCallback(Handler& _handler);
        void SendtoHandler(std::shared_ptr<Message> msg);
    private:
        Handler& mparent;
    };

private:
    std::shared_ptr<MessageQueue> messageQueue;
    std::shared_ptr<Looper> mlooper;
    std::shared_ptr<LooperCallback> mloopercb;
    int mid;
    std::thread looperThread;

public:
    explicit Handler(int id);
    void start();
    void join();
    void post(const std::shared_ptr<Message>& message);
    virtual void handleMessage(std::shared_ptr<Message> msg) = 0;
};

#endif // HANDLER_H
