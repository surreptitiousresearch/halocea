#include "animINST.h"

// animINST::GetTopObj @ 0x823D05C0 (?GetTopObj@animINST@@QBAPAVobjOBJ@@XZ) — return the model
// object list head (pObj, offset 0x148), i.e. the instance's root object.
//
// This is the target of the hcex bridge's flattened externs animINST_get_pObj and
// animINST_GetObjRoot (both resolve here).
objOBJ *animINST::GetTopObj() const
{
    return this->pObj;
}
