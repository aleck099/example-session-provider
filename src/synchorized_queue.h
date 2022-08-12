#ifndef EXSP_SYNCHORIZED_QUEUE
#define EXSP_SYNCHORIZED_QUEUE

#include <queue>
#include <mutex>

template<typename F, typename... Args>
concept callable = requires (F f, Args... args) {
    f(args...);
};

template<typename T>
class synchorized_queue {
public:
    std::queue<T> q;
    std::mutex mtx;

    std::unique_lock<std::mutex> new_unique_lock() {
        return std::unique_lock<std::mutex>{ mtx };
    }

    template<callable F>
    void synchorized(F f) {
        auto l = new_unique_lock();
        f();
    }
};

#endif

