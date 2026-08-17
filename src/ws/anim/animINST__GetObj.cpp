/* ?GetObj@animINST@@QBAPAVobjOBJ@@H@Z @0x823CD930 */
#include "animINST.h"
#include "../ds/ds_assert_boundary.h"

// ?GetObj@animINST@@QBAPAVobjOBJ@@H@Z  (0x823CD930) — const (QBA). Return the model object at
// index `id`, bounds-checked by the strong-assert path. objOBJ stays opaque (pointer only).
objOBJ *animINST::GetObj(int id) const
{
    if (!IGNORE_STRONG_ASSERT && (id < 0 || id >= this->nObj))
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
            "id >=0 && id < nObj",
            "D:\\Projects\\code\\common\\INCL.SYS\\anim.h",
            834,
            empty_string);
    return this->ppObjList[id];
}
