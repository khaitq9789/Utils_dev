#include "Looper.h"


Looper::Looper(const std::shared_ptr<MessageQueue>& mq, std::shared_ptr<ILooperCallback> _mHandlercb) : messageQueue(mq), mHandlercb(_mHandlercb) {}

void Looper::loop() {
    while (true) {
        auto message = messageQueue->next();
        if (message == nullptr) break;
        mHandlercb->SendtoHandler(message);
    }
}

