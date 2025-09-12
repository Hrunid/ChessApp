#include "VSComputer.h"

VsComputer::VsComputer(GameType type, UI& uiReff, int cpuLv)
    :   Game(type, uiReff),
        computerStrength(cpuLv)
        {
            
        }