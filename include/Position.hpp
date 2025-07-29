#ifndef POSITION_H
#define POSITION_H

#include <string>

struct Position{
    int x;
    int y;
    Position(int x, int y)
    :   x(x),
        y(y)
        {}
    
    std::string convertPositionTo(){
        char x = 'a' + x; 
        char y = '8' + y;
        std::string s = "";
        s += x;
        s += y;
        return s;
    }
};

inline bool operator==(const Position& a, const Position& b) {
    return a.x == b.x && a.y == b.y;
}
inline bool operator!=(const Position& a, const Position& b){
    return !(a == b);
}
inline Position operator+(const Position& a, const Position& b){
    return Position(a.x+b.x, a.y+b.y);
}
inline Position operator+=(const Position& a,const Position& b){
    return Position(a.x+b.x, a.y+b.y);
}
inline Position& operator+=(Position& pos, const std::pair<int, int> pair){
    pos.x += pair.first;
    pos.y += pair.second;
    return pos;
}

#endif