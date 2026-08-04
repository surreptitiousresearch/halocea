// aiTASK_DATA_FN::Init(const dsTSTRING<char>&, dsSTRID&) @ 0x83260788
// ?Init@aiTASK_DATA_FN@@UAAXABV?$dsTSTRING@D@@AAVdsSTRID@@@Z
#include "../../headers/ws/ai/aiTASK_DATA_FN.h"

// Virtual init that defers object resolution to run time (OBJ_BY_NAME); no function pointer yet.
void aiTASK_DATA_FN::Init(const dsTSTRING<char> &objName_, dsSTRID &fnName_)
{
    objName = objName_;                   // dsTSTRING refcounted assign (self-assign guarded)
    objType = AI_TASK_DATA_OBJ_BY_NAME;
    fnName.id = fnName_.id;               // dsSTRID copies only the interned id
}
