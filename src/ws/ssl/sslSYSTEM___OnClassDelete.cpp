#include "sslSYSTEM.h"
#include "../ds/dsCMP.h"

// 0x825CDF40 — sslSYSTEM::_OnClassDelete. Class-deleted callback: locate `uid` in the sorted
// classNames registry and, if present, erase its entry. The decompiler's a3..a14 are phantom GPR
// args (the sret/by-ref shuffle of the FindSorted key); the real signature takes a single int uid.
void sslSYSTEM::_OnClassDelete(int uid)
{
    dsCMP cmp;
    int idx = classNames.FindSorted<dsCMP, int>(uid, cmp);
    if (idx != -1)
        classNames.Erase(idx, 1);
}
