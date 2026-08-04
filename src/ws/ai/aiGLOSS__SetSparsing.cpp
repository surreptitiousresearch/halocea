#include "../../headers/ws/ai/aiGLOSS.h"
#include "../../headers/ws/wpn/wpnS_DESC.h"

// 0x8323ABA8 — aiGLOSS::SetSparsing. For a SUID-sparsed strike, (re)set the (SUID, teamShooter)
// cooldown timer and clear its playing flag.
void aiGLOSS::SetSparsing(const wpnS_DESC &desc, dsSTRID teamShooter, float time)
{
    if ( desc.typeSparse == wpnS_DESC::SPARSE_BY_SUID )
    {
        SparsingData *rec = FindCreateSparsing(desc.SUID, teamShooter);
        rec->timer = time;
        rec->isPlaying = false;
    }
}
