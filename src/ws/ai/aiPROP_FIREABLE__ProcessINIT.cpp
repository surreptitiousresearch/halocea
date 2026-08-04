#include "../../headers/ws/ai/aiPROP_FIREABLE.h"
#include "../../headers/ws/ai/aiPROP_FIREABLE_boundaries.h" // aifireable_OwnerIsBrain (gsIs<aiBRAIN>)
#include "../../headers/ws/dbg/STRONG_ASSERT_DUMMY.h"

extern int IGNORE_STRONG_ASSERT;    // _IGNORE_STRONG_ASSERT
extern unsigned char byte_8200155A; // shared empty-string / assert-info byte @0x8200155A

// aiPROP_FIREABLE::ProcessINIT() @ 0x832A3B08
// ?ProcessINIT@aiPROP_FIREABLE@@UAA_NXZ (virtual)
// Assert the owner is an aiBRAIN, then chain to the base fireable init.
bool aiPROP_FIREABLE::ProcessINIT()
{
    if (!IGNORE_STRONG_ASSERT && !aifireable_OwnerIsBrain(this->pOwner))
        STRONG_ASSERT_DUMMY::Crash(nullptr, "gsIs<aiBRAIN>(Owner(this))",
            "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_prop_fireable.cpp", 616,
            byte_8200155A);

    return propFIREABLE::ProcessINIT();
}
