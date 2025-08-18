#include "VSComputer.hpp"

VsComputer::VsComputer(GameType type, UI& uiReff, int cpuLv)
    :   Game(type, uiReff),
        computerStrength(cpuLv)
        {
            
        }