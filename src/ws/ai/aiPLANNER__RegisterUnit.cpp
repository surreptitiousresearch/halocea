// aiPLANNER::RegisterUnit @ 0x83246120
// ?RegisterUnit@aiPLANNER@@QAAXPAVaiBRAIN@@@Z
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiWAH_SYS.h"
#include "../../headers/ws/ai/aiWTC_HOLDER.h"
#include "../../headers/ws/dbg/STRONG_ASSERT_DUMMY.h"

extern int IGNORE_STRONG_ASSERT;
extern unsigned char STRONG_ASSERT_INFO_STUB; // byte_8200155A

// Register `pBrain` with the watcher subsystem as an active unit, unless it is already registered.
// The brain's aiBRAIN_IFACE subobject (offset 0) is the iaIACTOR the watcher demands a holder for.
void aiPLANNER::RegisterUnit(aiBRAIN *pBrain)
{
    if (!IGNORE_STRONG_ASSERT && !pBrain)
        STRONG_ASSERT_DUMMY::Crash(nullptr, "pBrain",
            "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_plan_msg.cpp", 306,
            STRONG_ASSERT_INFO_STUB);

    if (!wah.IsRegistered(pBrain))
    {
        aiWTC_HOLDER *holder = wah.Demand((iaIACTOR *)pBrain);
        wah.RegisterActive(holder);
    }
}
