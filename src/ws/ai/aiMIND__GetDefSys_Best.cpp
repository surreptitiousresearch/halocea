#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiBEST_BASE.h"
#include "../../headers/ws/ds/dsSHARED_PTR.h"
#include "../../headers/ws/ds/ds_new.h"

// aiMIND::GetDefSys_Best @ 0x8322AAE0
// ?GetDefSys_Best@aiMIND@@SA?AV?$dsSHARED_PTR@VaiBEST_BASE@@$0A@V?$Deleter@VaiBEST_BASE@@@@@@XZ
//   (static)
//
// The process-wide default best-behaviour module: a bare aiBEST_BASE constructed on first use and
// shared through a reference-counted pointer (Meyers singleton — the original guards a file-scope
// global with a one-time flag and an atexit destructor, which a function-local static reproduces).
dsSHARED_PTR<aiBEST_BASE, 0, Deleter<aiBEST_BASE> > aiMIND::GetDefSys_Best()
{
    static dsSHARED_PTR<aiBEST_BASE, 0, Deleter<aiBEST_BASE> > defBest(new aiBEST_BASE());
    return defBest;
}
