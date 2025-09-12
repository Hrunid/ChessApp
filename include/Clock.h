#ifndef CLOCK_H
#define CLOCK_H

#include <chrono>
#include <functional>
#include <thread>
#include <atomic>
#include <mutex>
#include <any>

using namespace std::chrono_literals;

class Clock{
    
private:
    using Seconds = std::chrono::duration<double>;

    Seconds time;
    Seconds increment;

    std::mutex mx;
    std::thread timer;
    std::atomic<bool> working{false};

    static constexpr Seconds interval = 0.04s;

public:
    inline Clock(int s = 0, int inc = 0)
    :   working(false),
        time(Seconds{s}),
        increment(Seconds{inc}),
        mx()
        {}

    inline ~Clock(){
        stop();
    }

    inline bool is_working(){
        return working.load(); 
    }
    inline int timeLeft(){
        std::lock_guard lock(mx);
        return (int)time.count(); 
    }
    inline bool runOut(){ 
        std::lock_guard lock(mx);
        return time < interval; 
    }
    inline void start(){
        if(working.load()) return;
        working = true;
        timer = std::thread([this](){
            auto end = time + std::chrono::steady_clock::now();
            auto next = interval + std::chrono::steady_clock::now();
            while(working.load() && !(runOut())){
                std::this_thread::sleep_until(next);
                next += interval;
                std::lock_guard time_lock(mx);
                time -= interval;
            }
            working.store(false, std::memory_order_relaxed);
        });
    };
    inline void stop(){
        working.store(false, std::memory_order_relaxed);
        if(timer.joinable()) timer.join();
    };
    inline void reset(){
        {
            std::lock_guard lock(mx);
            time = 0.0s;
        }
        stop();
    }
    inline void increment(){
        std::lock_guard lock(mx);
        time += increment;
    }
    inline std::any someFunc(std::any param){
        return param;
    }

};

#endif