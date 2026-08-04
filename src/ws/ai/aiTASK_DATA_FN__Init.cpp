// aiTASK_DATA_FN::Init(dsFUNC_PTR_BASE*, const dsTSTRING<char>&, const dsSTRID&) @ 0x83260718
// ?Init@aiTASK_DATA_FN@@UAAXPAVdsFUNC_PTR_BASE@@ABV?$dsTSTRING@D@@ABVdsSTRID@@@Z
#include "../../headers/ws/ai/aiTASK_DATA_FN.h"
#include "../../headers/ws/ds/dsFUNC_PTR_BASE.h"

// Virtual init with a pre-resolved scripted function pointer + object name + fn-name id (OBJ_GIVEN).
void aiTASK_DATA_FN::Init(dsFUNC_PTR_BASE *funcPtr_, const dsTSTRING<char> &objName_,
                          const dsSTRID &fnName_)
{
    funcPtr = funcPtr_;
    objName = objName_;                 // dsTSTRING refcounted assign (self-assign guarded)
    objType = AI_TASK_DATA_OBJ_GIVEN;
    fnName.id = fnName_.id;             // dsSTRID copies only the interned id
}
