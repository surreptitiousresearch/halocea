#include "dsDEQUE.h"
#include "dsStrIntMapERASED.h"

// dsDEQUE<...int-map ERASED>::PushBack @ 0x826197B0
// Grow the queue by one at the back, copy the deferred-erase record into the new slot,
// and return a reference to it.
template<>
dsStrIntMapERASED &dsDEQUE<dsStrIntMapERASED>::PushBack(const dsStrIntMapERASED &val)
{
    this->ResizeBack(this->length + 1);
    this->Back(); // decompiler emits a discarded Back() here (inlined bounds fetch)
    dsStrIntMapERASED *slot = this->Back();
    if (slot) {
        slot->threadMask = val.threadMask;
        slot->eraser.line = val.eraser.line;
        slot->eraser.index = val.eraser.index;
    }
    return *this->Back();
}
