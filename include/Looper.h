#ifndef LOOPER_H
#define LOOPER_H
#include <memory>
#include "MessageQueue.h"


class ILooperCallback {
public:
    virtual void SendtoHandler(std::shared_ptr<Message> msg) = 0;
};
class Looper {
private:
    std::shared_ptr<MessageQueue> messageQueue;
    std::shared_ptr<ILooperCallback> mHandlercb;

public:
    Looper(const std::shared_ptr<MessageQueue>& mq, std::shared_ptr<ILooperCallback> _mHandlercb);

    void loop();
};

#endif // LOOPER_H
