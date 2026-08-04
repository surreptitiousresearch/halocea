#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiSND_BASE.h"
#include "../../headers/ws/ds/dsSHARED_PTR.h"
#include "../../headers/ws/ds/ds_new.h"

// aiMIND::GetDefSys_Snd @ 0x8322AEF0
//   (static)  a8_mind.cpp:0x51
//
// Process-wide default module of type aiSND_BASE, constructed on first use and shared through a
// ref-counted pointer. The binary guards a file-scope global with a one-time flag and an atexit
// destructor; a function-local static reproduces that exactly.
dsSHARED_PTR<aiSND_BASE, 0, Deleter<aiSND_BASE> > aiMIND::GetDefSys_Snd()
{
    static dsSHARED_PTR<aiSND_BASE, 0, Deleter<aiSND_BASE> > defSnd(new aiSND_BASE());
    return defSnd;
}
