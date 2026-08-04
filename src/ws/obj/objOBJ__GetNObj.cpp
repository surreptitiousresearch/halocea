#include "objOBJ.h"
// 0x82681AF0  ?GetNObj@objOBJ@@QAAHP6AHPAV1@PAX@Z@Z
// Count the objects in this subtree (this node plus all descendants) that satisfy the
// predicate `cbCond`. A null predicate counts every object. Recurses the child sibling-ring.
//
// The decompiler typed the return as objOBJ* (it flows the int count through r3); the mangle
// (H return) and the disassembly (adds child counts, returns the accumulator) confirm int.

int objOBJ::GetNObj(StateCondProc cbCond)
{
    // `this` is null-tolerant on this path: a null node contributes zero.
    if (!this)
        return 0;

    int count = 0;
    objOBJ *firstChild = child;
    int started = 0;
    objOBJ *iter = firstChild;

    while (iter && (!started || iter != firstChild)) {
        count += iter->GetNObj(cbCond);
        iter = iter->next;
        started = 1;
    }

    if (!cbCond || cbCond(this, nullptr))
        ++count;
    return count;
}
