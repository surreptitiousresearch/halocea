#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiTASK_BASE.h"
#include "../../headers/ws/ds/dsSHARED_PTR.h"
#include "../../headers/ws/ds/ds_new.h"

// aiMIND::GetDefSys_Task @ 0x8322AFA0
//   (static)  a8_mind.cpp:0x56
//
// Process-wide default module of type aiTASK_BASE, constructed on first use and shared through a
// ref-counted pointer. The binary guards a file-scope global with a one-time flag and an atexit
// destructor; a function-local static reproduces that exactly.
dsSHARED_PTR<aiTASK_BASE, 0, Deleter<aiTASK_BASE> > aiMIND::GetDefSys_Task()
{
    static dsSHARED_PTR<aiTASK_BASE, 0, Deleter<aiTASK_BASE> > defTask(new aiTASK_BASE());
    return defTask;
}
