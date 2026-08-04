#include <string.h>
#include "dsVECTOR.h"
#include "ds_assert_boundary.h"
#include "../obj/objDEPENDENCY_CACHE.h" // declares ds::AppendFast<T>

class objOBJ; // boundary — obj subsystem game object

// ds::AppendFast<objOBJ*>(dsVECTOR<objOBJ*,8>&,const dsVECTOR<objOBJ*,8>&,int,int) @ 0x82688E48
// Bulk-append `right[start .. start+count)` onto `left` (count == -1 means "from start to the
// end of right"). Grows `left` first via the single-arg Resize(capacity) (append-only growth, no
// fill value needed since the new slots are immediately populated), then a flat memcpy of the
// pointer range — valid because the element type is a bare pointer.
//
// DEVIATION: the raw decompile shows 14 phantom trailing int parameters (a5..a18) — the usual
// float-slot-skip decompiler artifact seen throughout this corpus; the real signature is the
// 4-arg one declared in objDEPENDENCY_CACHE.h (matching its caller,
// objDEPENDENCY_CACHE::AddSharingUsers).
template<>
void ds::AppendFast<objOBJ *>(dsVECTOR<objOBJ *, 8> *left, const dsVECTOR<objOBJ *, 8> *right, int start, int count)
{
    int appendCount = count;
    if (count == -1) {
        appendCount = right->nElem - start;
        if (appendCount < 0)
            return;
    }
    if (!appendCount)
        return;

    if (!IGNORE_STRONG_ASSERT && start + appendCount > right->nElem)
        STRONG_ASSERT2_HELPER::asserd<int, int, int>(
            "start + count <= right.Length()",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_algorithm.hpp",
            373,
            "start", start, "count", appendCount, "start + count", start + appendCount);

    int oldCount = left->nElem;
    left->Resize(oldCount + appendCount);
    memcpy(&left->pData[oldCount], &right->pData[start], sizeof(objOBJ *) * appendCount);
}
