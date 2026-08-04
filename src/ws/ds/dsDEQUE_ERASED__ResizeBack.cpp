#include "dsDEQUE.h"
#include "dsStrStridMapERASED.h"

// dsDEQUE<...dsSTRID-map ERASED>::ResizeBack @ 0x8252AE40
// Grow the queue so it holds `size` elements at the back. If the backing store is too small it
// grows to 1.5x the requested size and re-lays the ring; then updates length and the tail index
// (or clears head/tail when empty). Element is the deferred-erase record of the dsSTRID-valued
// string map.
template<>
void dsDEQUE<dsStrStridMapERASED>::ResizeBack(int size)
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
