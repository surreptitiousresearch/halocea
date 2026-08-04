#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiAIM_BASE.h"
#include "../../headers/ws/ds/dsSHARED_PTR.h"
#include "../../headers/ws/ds/ds_new.h"

// aiMIND::GetDefSys_Aim @ 0x8322A980
// ?GetDefSys_Aim@aiMIND@@SA?AV?$dsSHARED_PTR@VaiAIM_BASE@@$0A@V?$Deleter@VaiAIM_BASE@@@@@@XZ
//   (static)
//
// The process-wide default aiming module: a bare aiAIM_BASE constructed on first use and shared
// through a reference-counted pointer (Meyers singleton — the original guards a file-scope global
// with a one-time flag and an atexit destructor, which a function-local static reproduces exactly).
dsSHARED_PTR<aiAIM_BASE, 0, Deleter<aiAIM_BASE> > aiMIND::GetDefSys_Aim()
{
    static dsSHARED_PTR<aiAIM_BASE, 0, Deleter<aiAIM_BASE> > defAim(new aiAIM_BASE());
    return defAim;
}
