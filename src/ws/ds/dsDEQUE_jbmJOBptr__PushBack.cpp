#include "dsDEQUE.h"

struct jbmJOB; // boundary — job-manager work item (element is a pointer)

// dsDEQUE<jbmJOB*>::PushBack @ 0x826EC6D8
// Grow by one at the back, store a copy of val into the new slot, return the slot.
template<>
jbmJOB *&dsDEQUE<jbmJOB *>::PushBack(jbmJOB *const &val)
{
    this->ResizeBack(this->length + 1);
    this->Back(); // decompiler emits a discarded Back() here (inlined bounds fetch)
    jbmJOB **slot = this->Back();
    if (slot)
        *slot = val;
    return *this->Back();
}
