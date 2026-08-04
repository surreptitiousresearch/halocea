#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiVISION_BASE.h"
#include "../../headers/ws/ai/ai_setup_subsystems_boundary.h" // aiVISION_IDLE / aiVISION_BATTLE
#include "../../headers/ws/ai/aiMIND_state_boundary.h"         // aiVISION_CHEAP
#include "../../headers/ws/ds/ds_new.h"

// aiMIND::DecideAppropriateVision @ 0x8322B1A8
// ?DecideAppropriateVision@aiMIND@@IAAXXZ  (protected)
//
// Honour a perception request to enhance or reduce the brain's vision fidelity. When enhancement is
// asked for and the current vision arm is the null/zero sentinel, install a full vision module
// (idle vs battle depending on the mind state). When reduction is asked for and a real battle arm
// is active outside idle, drop to the cheaper battle variant. Either way the request flags are
// cleared.
void aiMIND::DecideAppropriateVision()
{
    aiBRAIN  *brain = static_cast<aiBRAIN *>(pBrain);
    aiSTATUS *st    = brain->st.pointee;

    if (st->perc.needToEnhanceVision)
    {
        if (!vision.pointee->IsReal())
        {
            if (IsIdle())
                vision.Reset<aiVISION_IDLE>(new aiVISION_IDLE(brain));
            else
                vision.Reset<aiVISION_BATTLE>(new aiVISION_BATTLE(brain));
        }
        st->perc.needToEnhanceVision = false;
        st->perc.needToReduceVision  = false;
    }
    else if (st->perc.needToReduceVision)
    {
        if (vision.pointee->IsReal() && !IsIdle())
            // binary builds an aiVISION_BATTLE then repoints its vtable to aiVISION_CHEAP.
            vision.Reset<aiVISION_CHEAP>(new aiVISION_CHEAP(brain));
        st->perc.needToReduceVision = false;
    }
}
