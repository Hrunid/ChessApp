#ifndef VSCOMPUTER_H
#define VSCOMPUTER_H

#include "Game.h"

class VsComputer : public Game{
    private:
        int computerStrength;
        
    public:
        VsComputer(GameType type, UI& uiReff, int cpuLv);
};

#endif