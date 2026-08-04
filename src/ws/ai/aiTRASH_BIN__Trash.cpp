#include "../../headers/ws/ai/aiTRASH_BIN.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/dbg/STRONG_ASSERT_DUMMY.h"

extern int IGNORE_STRONG_ASSERT;    // _IGNORE_STRONG_ASSERT
extern unsigned char byte_8200155A; // shared empty-string / assert-info byte @0x8200155A

// aiTRASH_BIN::Trash(aiBRAIN*) @ 0x832AB508
// ?Trash@aiTRASH_BIN@@QAAXPAVaiBRAIN@@@Z
//
// Mark the owning brain itself for deferred removal (AI_DIEST 0x200). Only the bin's own brain may
// be trashed this way. If the brain is currently suspended (its message address will not tick), flush
// the bin immediately so the removal is not lost.
void aiTRASH_BIN::Trash(aiBRAIN *ptr)
{
    if (((ptr->st.pointee->die.val >> 9) & 1) == 0)
    {
        if (!IGNORE_STRONG_ASSERT && ptr != pBrain)
            STRONG_ASSERT_DUMMY::Crash(nullptr, "ptr == pBrain",
                "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_die.cpp", 189, byte_8200155A);

        ptr->st.pointee->die.val |= 0x200u; // AI_DIEST: pending removal
        if (ptr->suspendState)
            Update();
    }
}
