#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiVISION_BASE.h"
#include "../../headers/ws/ds/dsSHARED_PTR.h"
#include "../../headers/ws/ds/ds_new.h"

// aiMIND::GetDefSys_Vision @ 0x8322AE40
//   (static)  a8_mind.cpp:0x4C
//
// Process-wide default module of type aiVISION_BASE, constructed on first use and shared through a
// ref-counted pointer. The binary guards a file-scope global with a one-time flag and an atexit
// destructor; a function-local static reproduces that exactly.
dsSHARED_PTR<aiVISION_BASE, 0, Deleter<aiVISION_BASE> > aiMIND::GetDefSys_Vision()
{
    static dsSHARED_PTR<aiVISION_BASE, 0, Deleter<aiVISION_BASE> > defVision(new aiVISION_BASE());
    return defVision;
}
