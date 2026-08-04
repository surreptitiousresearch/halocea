#include "dsDEQUE.h"
#include "dsStrIntMapERASED.h"

// dsDEQUE<...int-map ERASED>::Reconstruct @ 0x82617310  (int-valued map variant; overload #2 by
// ascending address — matches the dsDEQUE<...>::PushBack tag ordering).
// Identical logic to the dsSTRID-valued variant: after the backing store grew from `oldSize` slots,
// relocate whichever wrapped ring run (head-run or tail-run) is smaller into the freshly added tail
// space so the queue is contiguous-ordered again. Elements are 12-byte
// {threadMask, eraser.line, eraser.index} records copied field-by-field.
template<>
void dsDEQUE<dsStrIntMapERASED>::Reconstruct(int oldSize)
{
    if (this->length <= 0)
        return;

    int tail = this->tail;
    int head = this->head;
    if (tail >= head)
        return; // already contiguous

    if (tail + 1 >= oldSize - head) {
        // head-run [head,oldSize) is the shorter side: move it into the enlarged tail space.
        dsStrIntMapERASED *back = &this->data.Back();
        int srcIndex = this->head;
        if (srcIndex < oldSize) {
            dsStrIntMapERASED *dst = &back[srcIndex - oldSize + 1];
            do {
                dsStrIntMapERASED *src = &this->data[srcIndex];
                if (dst) {
                    dst->threadMask = src->threadMask;
                    dst->eraser.line = src->eraser.line;
                    dst->eraser.index = src->eraser.index;
                }
                ++srcIndex;
                ++dst;
            } while (srcIndex < oldSize);
        }
        this->head += this->data.nElem - oldSize;
    } else {
        // tail-run [0,tail] is the shorter side: shift it up by oldSize slots.
        dsStrIntMapERASED *dst = (&this->data.Front() + oldSize);
        for (int srcIndex = 0; srcIndex <= this->tail; ++dst) {
            dsStrIntMapERASED *src = &this->data[srcIndex];
            if (dst) {
                dst->threadMask = src->threadMask;
                dst->eraser.line = src->eraser.line;
                dst->eraser.index = src->eraser.index;
            }
            ++srcIndex;
        }
        this->tail += oldSize;
    }
}
