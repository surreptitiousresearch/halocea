#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiISA_BASE.h"
#include "../../headers/ws/ai/ai_setup_subsystems_boundary.h" // aiISA_ZERO / aiVISION_BATTLE / aiAIM_BATTLE
#include "../../headers/ws/ds/ds_new.h"

// aiMIND::ZeroISA @ 0x8322B600
// ?ZeroISA@aiMIND@@QAAXXZ
//
// Reset the interop/social ("isa"), vision and aim modules back to their neutral battle/zero
// variants and clear the corresponding mind-state flag (0x8). Called when the brain leaves an
// interop-driven state.
void aiMIND::ZeroISA()
{
    if (isa.pointee && isa.pointee->IsZero())
        return;

    aiBRAIN *brain = static_cast<aiBRAIN *>(pBrain);

    // binary builds an aiISA_IFACE then repoints its vtable to aiISA_ZERO.
    isa.Reset<aiISA_ZERO>(new aiISA_ZERO(brain));
    vision.Reset<aiVISION_BATTLE>(new aiVISION_BATTLE(brain));
    aim.Reset<aiAIM_BATTLE>(new aiAIM_BATTLE(brain));

    brain->st.pointee->mind.val &= ~8u;
}
