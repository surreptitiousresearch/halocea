/* ?GetSubNameIdx@sslCLASS@@QAAHVdsSTRID@@@Z @0x82A38870 */
#include "sslCLASS.h"
#include "sslCLASS_NAME_TO_IDX.h"
#include "../ds/dsSTRID.h"

// boundary — comparator over interned name ids for the ascending-sorted nameToElemIdx vector
// (the actual comparison is inlined by dsVECTOR::FindSorted).
struct dsCMP {};

// 0x82A38870 — sslCLASS::GetSubNameIdx. Walk this class and its parents for a sub-element named
// `name`; return its element index, or -1. The decompiler's trailing phantom float-ABI args are
// dropped (the true signature takes a single by-value dsSTRID).
int sslCLASS::GetSubNameIdx(dsSTRID name)
{
    for (sslCLASS *cls = this; cls; cls = cls->refParent.pClass)
    {
        dsCMP cmp = dsCMP();
        int idx = cls->nameToElemIdx.FindSorted<dsCMP, dsSTRID>(name, cmp);
        if (idx != -1)
            return cls->nameToElemIdx[idx].idx;
    }
    return -1;
}
