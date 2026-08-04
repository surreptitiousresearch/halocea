#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiWAH_boundaries.h"

// aiWATCHER_BRAIN::GetMaxRangedEnemies @ 0x83170368
// ?GetMaxRangedEnemies@aiWATCHER_BRAIN@@UBAHXZ
// Const virtual override. 888 when the watcher is no longer valid; otherwise read the cap from the
// tracked brain's unit descriptor (a8_plan_wah "desc"; boundary raw offset).
// NOTE: the decompiler mistypes the return as dscBRAND*; disassembly (lwz r3, 0x2FC) shows a plain
// 32-bit int load — matches the UBAHXZ signature.
int aiWATCHER_BRAIN::GetMaxRangedEnemies() const
{
    if (!const_cast<aiWATCHER_BRAIN *>(this)->IsValid())
        return 888;

    aiBRAIN *pBrain = brain.pHandle ? brain.pHandle->pPtr : nullptr;
    const char *desc = *reinterpret_cast<const char *const *>(
        reinterpret_cast<const char *>(pBrain) + aiBRAIN_off_unitDesc);

    if (!IGNORE_STRONG_ASSERT && !desc)
        STRONG_ASSERT_DUMMY::Crash(nullptr, "desc",
            "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_plan_wah.cpp",
            790, byte_8200155A);

    return *reinterpret_cast<const int *>(desc + aiUNITDESC_off_maxRangedEnemies);
}
