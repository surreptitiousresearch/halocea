#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"

// aiMIND::EnsureDoneInitialSetup @ 0x8322C4C0
// ?EnsureDoneInitialSetup@aiMIND@@QAA_NXZ
//
// If the brain's "needs initial setup" status bit (gen bit 5) is still set, run SetupInitial and
// report whether the bit is now clear. Returns true when initial setup is already done or has just
// completed; false only if the bit remains set after SetupInitial.
bool aiMIND::EnsureDoneInitialSetup()
{
    aiBRAIN *brain = static_cast<aiBRAIN *>(pBrain);

    if (((brain->st.pointee->gen.val >> 5) & 1) == 0)
        return true;

    SetupInitial();

    if (((brain->st.pointee->gen.val >> 5) & 1) == 0)
        return true;
    return false;
}
