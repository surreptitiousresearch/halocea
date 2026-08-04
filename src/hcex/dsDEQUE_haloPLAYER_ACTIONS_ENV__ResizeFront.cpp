#include "../headers/ws/ds/dsDEQUE.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

// dsDEQUE<haloPLAYER_ACTIONS_ENV>::ResizeFront(int size) @ 0x823C09E8
// Grow (never shrink the backing) and set the ring so it holds `size` elements, trimming/extending
// at the front: the head retreats to tail-size+1, wrapping into the capacity when negative.
template<>
void dsDEQUE<haloPLAYER_ACTIONS_ENV>::ResizeFront(int size)
{
    if (size > this->data.nElem)
    {
        int oldSize = this->data.nElem;
        this->data.Resize(3 * size / 2);
        this->Reconstruct(oldSize);
    }
    if (size)
    {
        this->head = this->tail - size + 1;
        if (this->head < 0)
            this->head += this->data.nElem;
    }
    else
    {
        this->tail = 0;
        this->head = 0;
    }
    this->length = size;
}
