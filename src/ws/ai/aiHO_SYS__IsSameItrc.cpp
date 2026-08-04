#include "../../headers/ws/ai/aiHO_SYS.h"
#include "../../headers/ws/mdl/mdlITRC_BASE.h"  // mdlITRC_BASE::IsHO / pointee

// aiHO_SYS::IsSameItrc @ 0x832A7710
// ?IsSameItrc@aiHO_SYS@@MBA_NV?$dsSHARED_PTR@...@@0@Z
//
// Two handles denote the same hideout when both are valid HO ITRCs referring to the same underlying
// corner. Consumes both handles (mirrors the binary's explicit Reset on the by-value copies).
bool aiHO_SYS::IsSameItrc(ItrcPtr itrc1, ItrcPtr itrc2) const
{
    bool result = false;

    if (itrc1.pointee && itrc2.pointee && itrc1.pointee->IsHO() && itrc2.pointee->IsHO()) {
        // Corner identity: each ITRC references its owning HO host at +8 (mdlITRC_HO layout is the
        // next frontier); the hosts match when their identity token at +0x128 is equal. Read through
        // raw offsets rather than fabricated fields.
        char *host1 = *reinterpret_cast<char **>(reinterpret_cast<char *>(itrc1.pointee) + 8);
        char *host2 = *reinterpret_cast<char **>(reinterpret_cast<char *>(itrc2.pointee) + 8);
        result = (*reinterpret_cast<void **>(host1 + 0x128) == *reinterpret_cast<void **>(host2 + 0x128));
    }

    itrc1.Reset();
    itrc2.Reset();
    return result;
}
