#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
#include "Message.h"

class MessageQueue {
public:
    void enqueueMessage(const std::shared_ptr<Message>& message);
    std::shared_ptr<Message> next();
    void quit();
private:
    std::queue<std::shared_ptr<Message>> queue;
    std::mutex mutex;
    std::condition_variable condVar;
    bool isRunning = true;
};

#endif // MESSAGEQUEUE_H
