#include "../../headers/ws/ai/aiGLOSS.h"

// 0x8323A4D0 (const overload) / 0x8323A5A8 (mutable overload) — aiGLOSS::FindSparsing.
// Linear scan of the sparsings vector for the record matching (SUID, teamShooter). The two
// overloads share an identical body; teamShooter equality is an interned-id pointer compare.

const aiGLOSS::SparsingData *aiGLOSS::FindSparsing(int SUID, dsSTRID teamShooter) const
{
    int nElem = sparsings.nElem;
    for ( int i = 0; i < nElem; ++i )
    {
        const SparsingData *rec = &sparsings[i]; // bounds-checked (STRONG_ASSERT idx<nElem inlined)
        if ( rec->SUID == SUID && rec->teamShooter.id == teamShooter.id )
            return rec;
    }
    return nullptr;
}

aiGLOSS::SparsingData *aiGLOSS::FindSparsing(int SUID, dsSTRID teamShooter)
{
    int nElem = sparsings.nElem;
    for ( int i = 0; i < nElem; ++i )
    {
        SparsingData *rec = &sparsings[i];
        if ( rec->SUID == SUID && rec->teamShooter.id == teamShooter.id )
            return rec;
    }
    return nullptr;
}
