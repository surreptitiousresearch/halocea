#include "animINST.h"
#include <new>

// animINST::Clear @ 0x825E4028 (static)
// Reset an instance to its just-constructed state by destroying it in place and
// re-running the default constructor over the same storage.
void animINST::Clear(animINST *pInst)
{
    pInst->~animINST();
    if (pInst)
        new (pInst) animINST(); // in-place reconstruct (decompiler: animINST::animINST(pInst))
}
