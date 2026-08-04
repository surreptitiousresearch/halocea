#include "../headers/ws/ds/dsDEQUE.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

// dsDEQUE<haloPLAYER_ACTIONS_ENV>::ResizeBack(int size) @ 0x823C08F8
// Grow (never shrink the backing) and set the ring so it holds `size` elements, extending at the
// back: the tail advances to head+size-1 wrapped modulo capacity.
template<>
void dsDEQUE<haloPLAYER_ACTIONS_ENV>::ResizeBack(int size)
{
    if (size > this->data.nElem)
    {
        int oldSize = this->data.nElem;
        this->data.Resize(3 * size / 2);
        this->Reconstruct(oldSize);
    }
    if (size)
    {
        // DEVIATION: the __twllei/__twlgei intrinsics the PPC decompiler emits are the trap-on-
        // divide-by-zero / overflow guards the compiler inserts around a signed modulo; the source
        // is a plain `%`.
        this->tail = (this->head + size - 1) % this->data.nElem;
    }
    else
    {
        this->head = 0;
        this->tail = 0;
    }
    this->length = size;
}
