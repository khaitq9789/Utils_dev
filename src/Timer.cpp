#include <Timer.h>


Timer::Timer(std::shared_ptr<ITimerHandler> cb) {
    isRunning = false;
    isTimerActive = false;
    mcb = cb;
}

Timer::~Timer() {
    stop();
    {
        std::lock_guard<std::mutex> lock(mutex);
        isRunning = false;
    }
    condition.notify_one();
    if (loop.joinable()) {
        loop.join();
    }
}

void Timer::setup(int timeout, bool repeat = false) {
    this->timeoutMs = timeout;
    this->isRepeat = repeat;
}

void Timer::start() {
    std::lock_guard<std::mutex> lock(mutex);
    loop = std::thread([this]() { Looper();});
    this->isTimerActive = true;
    condition.notify_one();
}
void Timer::stop() {
    std::lock_guard<std::mutex> lock(mutex);
    isTimerActive = false;
}
void Timer::Looper() {
    isRunning = true;
    std::unique_lock<std::mutex> lock(mutex);
    while (isRunning) {
        condition.wait(lock, [this]() { return isTimerActive || !isRunning; });
        if (!isRunning) break;
        do {
            lock.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(timeoutMs));
            lock.lock();

            if (isTimerActive && mcb) {
                mcb->handleTimerExpired();
            }
        } while (isRepeat && isTimerActive);
        isTimerActive = false;
    }
}