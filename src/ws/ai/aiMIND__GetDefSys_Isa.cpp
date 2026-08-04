#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiISA_BASE.h"
#include "../../headers/ws/ds/dsSHARED_PTR.h"
#include "../../headers/ws/ds/ds_new.h"

// aiMIND::GetDefSys_Isa @ 0x8322AD90
//   (static)  a8_mind.cpp:0x47
//
// Process-wide default module of type aiISA_BASE, constructed on first use and shared through a
// ref-counted pointer. The binary guards a file-scope global with a one-time flag and an atexit
// destructor; a function-local static reproduces that exactly.
dsSHARED_PTR<aiISA_BASE, 0, Deleter<aiISA_BASE> > aiMIND::GetDefSys_Isa()
{
    static dsSHARED_PTR<aiISA_BASE, 0, Deleter<aiISA_BASE> > defIsa(new aiISA_BASE());
    return defIsa;
}
