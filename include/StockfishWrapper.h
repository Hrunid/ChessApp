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

    void print() const{
        std::cout << "Depth: " << depth << " Eval: " << evalCp << " Mate: " << isMate << " Line: " << moves << std::endl;
    }
};

class Sf_Wrapper{
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

    void setLiveUpdate() {
        
        sf.set_on_update_full([this](const Stockfish::Engine::InfoFull& info) noexcept {

            const size_t idx = info.multiPV ? size_t(info.multiPV - 1) : 0;
            if (idx >= lines.size()) return;

            std::unique_lock<std::shared_mutex> lock(lines_mx);
            Line& L = lines[idx];

            if(std::string_view pv = info.pv; L.moves.size() < pv.size()){
                info.score.visit([&](const auto& sc) noexcept {
                    using T = std::decay_t<decltype(sc)>;
                    if constexpr (std::is_same_v<T, Stockfish::Score::InternalUnits>) L.evalCp = double(sc.value);
                    else { L.isMate = true; L.mateMoves = (sc.plies + 1) / 2; }
                });
                L.moves.assign(pv.begin(), pv.end());
            }
                
        });

        sf.set_on_bestmove([this](std::string_view, std::string_view){
            working = false;
        });
        sf.set_on_iter([](Stockfish::Search::InfoIteration iter){});
        sf.set_on_update_no_moves([](Stockfish::Search::InfoShort){});
        sf.set_on_verify_networks([](std::string_view){});
    }    

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
    sf_Wrapper()
    :   sf(),
        opt(sf.get_options()),
        max_threads(std::thread::hardware_concurrency() ? std::thread::hardware_concurrency() : 1),
        max_RAM(avail_RAM()),
        lines(1),
        desiredDepth(20),
        lim(),
        working(false)
    {
        setUsedRAM(128);
        setUsedThreads(1);
        setLiveUpdate();
    }

    static void init(){
        Stockfish::Bitboards::init();
        Stockfish::Position::init();
    }

    static uint64_t avail_RAM(){
        MEMORYSTATUSEX statex;
        statex.dwLength = sizeof(statex);
        if (GlobalMemoryStatusEx(&statex)) {
            
            return mb(statex.ullAvailPhys);
        }
        return 256;
    }

    static int mb(uint64_t bytes){ 
        return bytes / 1024 / 1024;
    }

    int  threads() const        { return (int)opt["Threads"]; }
    int  hashMB() const         { return (int)opt["Hash"]; }
    int  multiPV() const        { return (int)opt["MultiPV"]; }
    bool limitStrength() const  { return (int)opt["UCI_LimitStrength"] != 0; }
    int  elo() const            { return (int)opt["UCI_Elo"]; }
    int  skill() const          { return (int)opt["Skill Level"]; }    
    bool isWorking()            { return working; }

    void setUsedThreads(int t_count){
        if(t_count <= 0 || t_count > max_threads) throw std::invalid_argument("Can not assign " + std::to_string(t_count) + " threads!" );
        setOpt("Threads", t_count);
    }
    void setUsedRAM(int ram_mb){
        if(ram_mb <= 0 || ram_mb > max_RAM) throw std::invalid_argument("Can not assign " + std::to_string(ram_mb) + " RAM!" );
        setOpt("Hash", ram_mb);
    }
    void setElo(int elo){
        if(1320 > elo || elo > 3190) throw std::invalid_argument("Can not set elo  " + std::to_string(elo));
        setOpt("UCI_LimitStrength", true);
        setOpt("UCI_Elo", elo);
    }
    void setSkill(int skill){
        if(0 > skill || skill > 20) throw std::invalid_argument("Can not set level  " + std::to_string(skill));
        setOpt("UCI_LimitStrength", false);
        setOpt("Skill Level", skill);
    }
    void setMultiPV(int c){
        if(0 > c || c > 4) throw std::invalid_argument("Can not calculate " + std::to_string(c) + " lines!" );
        {
        std::unique_lock<std::shared_mutex> lk(lines_mx);
        lines.resize(c, Line{});
        }   
        setOpt("MultiPV", c);
    }
    void setPosition(const std::string& fen = "startpos", const std::vector<std::string>& moves = {}){
        sf.set_position(fen, moves);
    }
    void setDesiredDepth(int desDepth){
        if(0 > desDepth || desDepth > 99) throw std::invalid_argument("Can not use depth " + std::to_string(desDepth));
        desiredDepth = desDepth;
    }
    void startDepth(){
        lim.depth = desiredDepth;
        working = true;
        Stockfish::Search::LimitsType limit{};
        limit.depth = desiredDepth;
        {
            std::lock_guard lk(engine_mx);
            sf.go(limit);
        }
         
    }
    void startTime(int ms){
        lim.movetime = ms;
        working = true;
        Stockfish::Search::LimitsType limits{};
        limits.depth = 99;
        {
            std::lock_guard lk(engine_mx);
            sf.go(limits);
        }

        std::thread timer{[&](int ms){
            clock_working = true;
            search_start = std::chrono::steady_clock::now();
            auto end = search_start + std::chrono::milliseconds{ms};
            while(working && (std::chrono::steady_clock::now() < end && clock_working)){
                std::this_thread::sleep_until(std::chrono::steady_clock::now() + 100ms);
            }
            stop();
        }};
        timer.detach();
    }

    void stop(){
        working = false;
        clock_working = false;
        {
            std::lock_guard lk(engine_mx);
            sf.stop();
        }
        sf.wait_for_search_finished();
    }
    void waitForSearchFinished(){
        sf.wait_for_search_finished();
    }
    std::vector<Line>* get_lines_ptr(){
        return &lines;
    }
    std::shared_mutex* get_mx_ptr(){
        return &lines_mx;
    }
};