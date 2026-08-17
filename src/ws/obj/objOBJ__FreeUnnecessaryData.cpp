/* ?FreeUnnecessaryData@objOBJ@@QAAXXZ @0x82A7F5C0 */
#include "objOBJ.h"
// 0x82A7F5C0  ?FreeUnnecessaryData@objOBJ@@QAAXXZ
// Recurse over the child sibling-ring, freeing unnecessary data on every descendant. The
// (!started || iter != firstChild) guard walks the circular sibling list exactly once.

void objOBJ::FreeUnnecessaryData()
{
    objOBJ *firstChild = nullptr;
    int started = 0;
    objOBJ *iter = child; // `this` is always non-null on the reversed entry path
    firstChild = iter;

    while (iter && (!started || iter != firstChild)) {
        iter->FreeUnnecessaryData();
        iter = iter->next;
        started = 1;
    }
}
