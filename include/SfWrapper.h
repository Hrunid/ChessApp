#include <string>
#include <optional>
#include <thread>
#include <windows.h>
#include <cstdint>
#include <sstream>
#include <type_traits>
#include <vector>
#include <chrono>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>

#include "bitboard.h"
#include "misc.h"
#include "position.h"
#include "types.h"
#include "uci.h"
#include "tune.h"
#include "engine.h"
#include "ucioption.h"
#include "score.h"
#include "search.h"

using namespace std::chrono_literals;

struct Line{
    int depth = 0;
    double evalCp = 0;
    bool isMate = false;
    int mateMoves = 0;
    std::string moves;
};

class SfWrapper{
private:
    Stockfish::Engine sf;
    Stockfish::OptionsMap& opt;

    std::shared_mutex lines_mx;
    mutable std::mutex engine_mx;

    Stockfish::Search::LimitsType lim;
    std::atomic<bool> working;

    std::chrono::time_point<std::chrono::steady_clock> search_start;
    std::atomic<bool> clock_working;

    int max_threads;
    int max_RAM;
    int desiredDepth;
    std::vector<Line> lines;

    void setLiveUpdate();

    template<typename T>
    void setOpt(std::string&& optName, T value){
        if(working && lim.depth > 0){
            this->stop();
            set(optName, value);
            startDepth();
        }
        else{
            set(optName, value);
        }
    }
    template<typename T>
    void set(const std::string& optName, T value){
        std::string t;
        t.reserve(11);
        if constexpr (std::is_same_v<T, bool>) {
            t = value ? "true" : "false";
        } else {
            t = std::to_string(value);
        }
        std::istringstream s
            (std::string("name ") 
                + std::move(optName) 
                + std::string(" value ")
                + t
            );
        std::lock_guard eng_lk(engine_mx);
        opt.setoption(s);
    }

public:
    SfWrapper() = default;
    static void init();
    static uint64_t avail_RAM();
};