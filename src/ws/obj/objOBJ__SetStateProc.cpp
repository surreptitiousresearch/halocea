/* ?SetStateProc@objOBJ@@QAAXHHP6AHPAV1@PAX@Z@Z @0x82682430 */
#include "objOBJ.h"
// 0x82682430  ?SetStateProc@objOBJ@@QAAXHHP6AHPAV1@PAX@Z@Z
// Set or clear the `stateProc` bits on this object and, recursively, on every sibling in its
// child ring. When `cbCond` is supplied it must return true for a node before that node's bits
// are touched. `enable` selects set (non-zero) vs clear (zero).

void objOBJ::SetStateProc(int stateProc, int enable, StateCondProc cbCond)
{
    if (!this)
        return;

    objOBJ *firstChild = this->child;
    if (!cbCond || cbCond(this, nullptr))
        this->stateProc = (this->stateProc & ~stateProc) | (enable == 0 ? 0 : stateProc);

    bool visited = false;
    for (objOBJ *node = firstChild; node && (!visited || node != firstChild); node = node->next) {
        node->SetStateProc(stateProc, enable, cbCond);
        visited = true; // stop once the circular sibling ring loops back to firstChild
    }
}
