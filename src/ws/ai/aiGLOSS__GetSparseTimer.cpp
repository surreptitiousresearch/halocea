#include "../../headers/ws/ai/aiGLOSS.h"

// 0x8323A8D0 — aiGLOSS::GetSparseTimer. Remaining sparse cooldown for (SUID, teamShooter); 0 when
// no record exists.
float aiGLOSS::GetSparseTimer(int SUID, dsSTRID teamShooter) const
{
    const SparsingData *rec = FindSparsing(SUID, teamShooter);
    return rec ? rec->timer : 0.0f;
}
