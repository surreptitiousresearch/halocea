#include "dsDEQUE.h"
#include "dsStrStridMapERASED.h"

// dsDEQUE<...dsSTRID-map ERASED>::PushBack @ 0x8252AF88
// Grow the queue by one at the back, copy the deferred-erase record into the new slot,
// and return a reference to it.
template<>
dsStrStridMapERASED &dsDEQUE<dsStrStridMapERASED>::PushBack(const dsStrStridMapERASED &val)
{
    this->ResizeBack(this->length + 1);
    this->Back(); // decompiler emits a discarded Back() here (inlined bounds fetch)
    dsStrStridMapERASED *slot = this->Back();
    if (slot) {
        slot->threadMask = val.threadMask;
        slot->eraser.line = val.eraser.line;
        slot->eraser.index = val.eraser.index;
    }
    return *this->Back();
}
