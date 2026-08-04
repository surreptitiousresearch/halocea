#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiMP_BASE.h"
#include "../../headers/ws/ds/dsSHARED_PTR.h"
#include "../../headers/ws/ds/ds_new.h"

// aiMIND::GetDefSys_Mp @ 0x8322AC30
//   (static)  a8_mind.cpp:0x3D
//
// Process-wide default module of type aiMP_BASE, constructed on first use and shared through a
// ref-counted pointer. The binary guards a file-scope global with a one-time flag and an atexit
// destructor; a function-local static reproduces that exactly.
dsSHARED_PTR<aiMP_BASE, 0, Deleter<aiMP_BASE> > aiMIND::GetDefSys_Mp()
{
    static dsSHARED_PTR<aiMP_BASE, 0, Deleter<aiMP_BASE> > defMp(new aiMP_BASE());
    return defMp;
}
