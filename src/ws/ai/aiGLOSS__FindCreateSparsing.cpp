#include "../../headers/ws/ai/aiGLOSS.h"

// 0x8323AA30 — aiGLOSS::FindCreateSparsing. Return the existing (SUID, teamShooter) record, or
// append and return a fresh zeroed one on a miss.
aiGLOSS::SparsingData *aiGLOSS::FindCreateSparsing(int SUID, dsSTRID teamShooter)
{
    int nElem = sparsings.nElem;
    for ( int i = 0; i < nElem; ++i )
    {
        SparsingData *rec = &sparsings[i];
        if ( rec->SUID == SUID && rec->teamShooter.id == teamShooter.id )
            return rec;
    }

    SparsingData fresh;
    fresh.SUID = SUID;
    fresh.teamShooter.id = teamShooter.id;
    fresh.isPlaying = false;
    fresh.timer = 0.0f;
    return &sparsings.PushBack(fresh);
}
