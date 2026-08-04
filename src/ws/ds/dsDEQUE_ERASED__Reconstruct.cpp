#include "dsDEQUE.h"
#include "dsStrStridMapERASED.h"

// dsDEQUE<...dsSTRID-map ERASED>::Reconstruct @ 0x8252A658
// Called from ResizeBack after the backing dsVECTOR grew from `oldSize` slots. When the live contents
// were wrapped around the ring (tail < head), the newly appended tail space is used to make the queue
// contiguous again by relocating whichever of the two runs is smaller:
//   - if the head-run [head,oldSize) is the shorter side, copy it into the new tail space and advance
//     head by the number of added slots;
//   - otherwise the tail-run [0,tail] is shorter, so shift it up past oldSize and advance tail.
// Elements are 12-byte {threadMask, eraser.line, eraser.index} records copied field-by-field.
template<>
void dsDEQUE<dsStrStridMapERASED>::Reconstruct(int oldSize)
{
    if (this->length <= 0)
        return;

    int tail = this->tail;
    int head = this->head;
    if (tail >= head)
        return; // already contiguous

    if (tail + 1 >= oldSize - head) {
        // head-run [head,oldSize) is the shorter side: move it into the enlarged tail space.
        dsStrStridMapERASED *back = &this->data.Back();
        int srcIndex = this->head;
        if (srcIndex < oldSize) {
            dsStrStridMapERASED *dst = &back[srcIndex - oldSize + 1];
            do {
                dsStrStridMapERASED *src = &this->data[srcIndex];
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
        dsStrStridMapERASED *dst = (&this->data.Front() + oldSize);
        for (int srcIndex = 0; srcIndex <= this->tail; ++dst) {
            dsStrStridMapERASED *src = &this->data[srcIndex];
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
