#include "sslSYSTEM.h"
#include "sslCLASS_REF.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsPAIR.h"

// boundary — stateless three-way comparator over int keys (compare inlined in FindSorted).
struct dsCMP {};

// 0x825CF2B8 — sslSYSTEM::GetClassName. Binary-search the uid-sorted classNames registry for the
// class `c` refers to; return its registered name, or the shared empty string on a miss. Returns
// dsTSTRING<char> by value (sret): the decompiler swaps this/result (r3 = return slot dsTSTRING*,
// r4 = this sslSYSTEM*). `c` is passed by value (its class ref is released on return).
dsTSTRING<char> sslSYSTEM::GetClassName(sslCLASS_REF c) const
{
    int uid = c.GetUID();
    dsCMP cmp;
    int idx = this->classNames.FindSorted<dsCMP, int>(uid, cmp);

    dsTSTRING<char> name;
    if (idx == -1)
        name.UnsafeInitEmpty();     // shared empty-string singleton
    else
        name = this->classNames[idx].val;
    return name;
}
