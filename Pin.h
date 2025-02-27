#ifndef PIN_H
#define PIN_H

#include <utility>

struct Pin{

    int pinningPieceId;
    int pinnedPieceId;
    std::pair<int, int> pinningPieceDirection;
    std::pair<int, int> pinnedPieceDirection;

};

#endif