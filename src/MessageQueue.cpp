

#include "MessageQueue.h"


void MessageQueue::enqueueMessage(const std::shared_ptr<Message>& message) {
    std::lock_guard<std::mutex> lock(mutex);
    queue.push(message);
    condVar.notify_one();
}

std::shared_ptr<Message> MessageQueue::next() {
    std::unique_lock<std::mutex> lock(mutex);
    condVar.wait(lock, [this]() { return !queue.empty() || !isRunning; });

    if (!isRunning && queue.empty()) {
        return nullptr;
    }

    auto message = queue.front();
    queue.pop();
    return message;
}

void MessageQueue::quit() {
    std::lock_guard<std::mutex> lock(mutex);
    isRunning = false;
    condVar.notify_all();
}

