#include "../../headers/ws/ai/aiGLOSS.h"
#include "../../headers/ws/wpn/wpnS_DESC.h"

// 0x8323A800 — aiGLOSS::IsStrikeAvailable. A non-SUID-sparsed strike is always available; a
// SUID-sparsed one is available only when it is not currently playing and its cooldown has elapsed.
bool aiGLOSS::IsStrikeAvailable(const wpnS_DESC &desc, dsSTRID teamShooter) const
{
    if ( desc.typeSparse != wpnS_DESC::SPARSE_BY_SUID )
        return true;

    const SparsingData *rec = FindSparsing(desc.SUID, teamShooter);
    if ( !rec )
        return true;
    if ( rec->isPlaying )
        return false;
    return rec->timer < 0.000001f;
}
