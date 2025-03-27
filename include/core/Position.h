#ifndef POSITION_H
#define POSITION_H

struct Position{
    int x;
    int y;
    Position(int x, int y)
    :   x(x),
        y(y)
        {}
    
    std::string convertPositionTo(){
        char x = 'a' + x; 
        char y = '1' + y;
        std::string s = "";
        s += x;
        s += y;
        return s;
    }
};

#endif