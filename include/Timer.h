#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <thread>
#include <chrono>
#include <functional>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <memory>

class ITimerHandler {
public:
    virtual void handleTimerExpired() = 0;
};

class Timer {
public:
    explicit Timer(std::shared_ptr<ITimerHandler> cb);
    virtual ~Timer();
    void setup(int timeout, bool repeat = false);
    void start();
    void stop();
private:
    std::shared_ptr<ITimerHandler> mcb;
    std::atomic<bool> isRunning;
    bool isTimerActive;
    bool isRepeat;
    int timeoutMs;
    std::thread loop;
    std::condition_variable condition;
    std::mutex mutex;
    void Looper();
};

#endif // TIMER_H
