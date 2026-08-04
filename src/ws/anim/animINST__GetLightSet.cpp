#include "animINST.h"
#include "../ds/dsSMART_PTR.h"
#include "../ds/dsREF_COUNT.h"
#include "../rend/rendLIGHT_SET.h"
#include "anim_boundary.h"

// ?GetLightSet@animINST@@QAA?AV?$dsSMART_PTR@VrendLIGHT_SET@@...@@XZ  (0x825E3A58) — const (QBA).
// Returns a shared copy of the instance's held light-set smart pointer BY VALUE (sret): the return
// slot is r3, `this` is r4 (decompiler swapped their names). A null pointee, or the not-yet-resolved
// -4 sentinel, yields an empty smart pointer; otherwise the referent's intrusive count is bumped.
dsSMART_PTR<rendLIGHT_SET, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > animINST::GetLightSet() const
{
    dsSMART_PTR<rendLIGHT_SET, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > lightSet;
    rendLIGHT_SET *pointee = this->pLightSet.pointee; // raw referent held by the smart ptr
    if (pointee && pointee != reinterpret_cast<rendLIGHT_SET *>(-4))
        osLockedIncrement(&pointee->ref.refCount); // intrusive count at pointee+4
    else
        pointee = nullptr;
    lightSet.pointee = pointee;
    return lightSet;
}
