#include "dsDEQUE.h"

struct jbmJOB; // boundary — job-manager work item (element is a pointer)

// dsDEQUE<jbmJOB*>::ResizeBack @ 0x826EC558
// Set the live length to `size`, growing the backing store (to 1.5x) and re-laying the ring
// when it must expand, then recompute the tail index (or reset head/tail when emptied).
template<>
void dsDEQUE<jbmJOB *>::ResizeBack(int size)
{
    int oldElemCount = this->data.nElem;
    if (size > oldElemCount) {
        this->data.Resize(3 * size / 2);
        this->Reconstruct(oldElemCount);
    }
    this->length = size;
    if (size)
        this->tail = (this->head + size - 1) % this->data.nElem;
    else {
        this->head = 0;
        this->tail = 0;
    }
}
