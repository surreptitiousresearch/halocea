// aiPLANNER::ProcessSound @ 0x83246550
// ?ProcessSound@aiPLANNER@@IAAXPBVentSND_NOTICE@@@Z
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiWAH_SYS.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiISA_BASE.h"
#include "../../headers/ws/dbg/STRONG_ASSERT_DUMMY.h"

extern int IGNORE_STRONG_ASSERT;
extern unsigned char STRONG_ASSERT_INFO_STUB; // byte_8200155A

// Fan a sound notice out to every active AI unit: each watcher's brain hears it through its isa
// (interop/social) mind module.
void aiPLANNER::ProcessSound(const struct entSND_NOTICE *pNotice)
{
    if (!pNotice)
        return;

    int nUnits = wah.arrUnits.nElem;
    for (int i = 0; i < nUnits; ++i)
    {
        ds::WEAK_PTR_HANDLE<aiBRAIN> *handle = wah.PeekActive(i)->brain.pHandle;
        aiBRAIN *brain = handle ? handle->pPtr : nullptr;

        if (!IGNORE_STRONG_ASSERT && !brain)
            STRONG_ASSERT_DUMMY::Crash(nullptr, "pBrain",
                "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_plan_msg.cpp", 522,
                STRONG_ASSERT_INFO_STUB);

        brain->mind.isa.pointee->NotifySND(*pNotice);
    }
}
