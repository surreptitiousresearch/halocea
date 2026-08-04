#include "sslSYSTEM.h"
#include "../ds/dsCMP.h"

// 0x825CDEE0 — sslSYSTEM::_OnObjectDelete. Object-deleted callback: locate `uid` in the sorted
// objectNames registry and, if present, erase its entry. The decompiler's a3..a14 are phantom GPR
// args (the sret/by-ref shuffle of the FindSorted key); the real signature takes a single int uid.
void sslSYSTEM::_OnObjectDelete(int uid)
{
    dsCMP cmp;
    int idx = objectNames.FindSorted<dsCMP, int>(uid, cmp);
    if (idx != -1)
        objectNames.Erase(idx, 1);
}
