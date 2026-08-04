#include "dsDEQUE.h"
#include "dsStrIntMapERASED.h"

// dsDEQUE<...int-map ERASED>::ResizeBack @ 0x82618DA8
// Grow the queue so it holds `size` elements at the back. If the backing store is too small it
// grows to 1.5x the requested size and re-lays the ring; then updates length and the tail index
// (or clears head/tail when empty). Element is the deferred-erase record of the int-valued
// string map.
template<>
void dsDEQUE<dsStrIntMapERASED>::ResizeBack(int size)
{
    int oldCapacity = this->data.nElem;
    if (size > oldCapacity) {
        this->data.Resize(3 * size / 2);
        this->Reconstruct(oldCapacity);
    }
    this->length = size;
    if (size)
        this->tail = (this->head + size - 1) % this->data.nElem;
    else {
        this->head = 0;
        this->tail = 0;
    }
}
