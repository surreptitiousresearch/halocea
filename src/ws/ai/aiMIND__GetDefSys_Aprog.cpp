#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiAPPROACH_BASE.h"
#include "../../headers/ws/ds/dsSHARED_PTR.h"
#include "../../headers/ws/ds/ds_new.h"

// aiMIND::GetDefSys_Aprog @ 0x8322AA30
// ?GetDefSys_Aprog@aiMIND@@SA?AV?$dsSHARED_PTR@VaiAPPROACH_BASE@@$0A@V?$Deleter@VaiAPPROACH_BASE@@@@@@XZ
//   (static)
//
// The process-wide default approach ("aprog") module: a bare aiAPPROACH_BASE constructed on first
// use and shared through a reference-counted pointer (Meyers singleton — see GetDefSys_Aim).
dsSHARED_PTR<aiAPPROACH_BASE, 0, Deleter<aiAPPROACH_BASE> > aiMIND::GetDefSys_Aprog()
{
    static dsSHARED_PTR<aiAPPROACH_BASE, 0, Deleter<aiAPPROACH_BASE> > defAprog(new aiAPPROACH_BASE());
    return defAprog;
}
