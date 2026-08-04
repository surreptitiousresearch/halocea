#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiPERC_BASE.h"
#include "../../headers/ws/ds/dsSHARED_PTR.h"
#include "../../headers/ws/ds/ds_new.h"

// aiMIND::GetDefSys_Perc @ 0x8322ACE0
//   (static)  a8_mind.cpp:0x42
//
// Process-wide default module of type aiPERC_BASE, constructed on first use and shared through a
// ref-counted pointer. The binary guards a file-scope global with a one-time flag and an atexit
// destructor; a function-local static reproduces that exactly.
dsSHARED_PTR<aiPERC_BASE, 0, Deleter<aiPERC_BASE> > aiMIND::GetDefSys_Perc()
{
    static dsSHARED_PTR<aiPERC_BASE, 0, Deleter<aiPERC_BASE> > defPerc(new aiPERC_BASE());
    return defPerc;
}
