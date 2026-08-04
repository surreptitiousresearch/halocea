#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiGOAL_BASE.h"
#include "../../headers/ws/ds/dsSHARED_PTR.h"
#include "../../headers/ws/ds/ds_new.h"

// aiMIND::GetDefSys_Goal @ 0x8322AB90
// ?GetDefSys_Goal@aiMIND@@SA?AV?$dsSHARED_PTR@VaiGOAL_BASE@@$0A@V?$Deleter@VaiGOAL_BASE@@@@@@XZ
//   (static)  D:\Projects\code\common\src.sys\gm_shared\ai08\a8_mind.cpp:0x38
//
// Process-wide default goal module: an aiGOAL_BASE (0x38 bytes; its ctor initializes the
// invalidSpeed aiSPD member) constructed on first use and shared through a ref-counted pointer.
// The binary guards a file-scope global with a one-time flag and an atexit destructor — a
// function-local static reproduces that exactly.
dsSHARED_PTR<aiGOAL_BASE, 0, Deleter<aiGOAL_BASE> > aiMIND::GetDefSys_Goal()
{
    static dsSHARED_PTR<aiGOAL_BASE, 0, Deleter<aiGOAL_BASE> > defGoal(new aiGOAL_BASE());
    return defGoal;
}
