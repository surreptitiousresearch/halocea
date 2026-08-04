#include "dsDEQUE.h"

struct jbmJOB; // boundary — job-manager work item (element is a pointer)

// dsDEQUE<jbmJOB*>::Reconstruct @ 0x826EC0E0
// Called from ResizeBack after the backing dsVECTOR grew from `oldSize` slots. When the live
// contents were wrapped around the ring (tail < head), the newly appended tail space is used to
// make the queue contiguous again by relocating whichever of the two runs is smaller:
//   - if the head-run [head,oldSize) is the shorter side, shift it to the end of the enlarged store
//     and advance head by the number of added slots;
//   - otherwise the tail-run [0,tail] is shorter, so shift it up past oldSize and advance tail.
template<>
void dsDEQUE<jbmJOB *>::Reconstruct(int oldSize)
{
    if (this->length <= 0)
        return;

    int tail = this->tail;
    int head = this->head;
    if (tail >= head)
        return; // contents are already contiguous; nothing to relayout

    if (tail + 1 >= oldSize - head) {
        // head-run [head,oldSize) is the shorter side: move it into the new tail space.
        // Destination of the first moved element = &pData[nElem + head - oldSize].
        int srcIndex = head;
        if (srcIndex < oldSize) {
            jbmJOB **dst = &this->data.Back() + head + 1 - oldSize;
            do {
                jbmJOB **src = &this->data[srcIndex];
                if (dst)
                    *dst = *src;
                ++srcIndex;
                ++dst;
            } while (srcIndex < oldSize);
        }
        this->head += this->data.nElem - oldSize;
    } else {
        // tail-run [0,tail] is the shorter side: shift it up by oldSize slots.
        jbmJOB **dst = &this->data.Front() + oldSize;
        for (int srcIndex = 0; srcIndex <= this->tail; ++dst) {
            jbmJOB **src = &this->data[srcIndex];
            if (dst)
                *dst = *src;
            ++srcIndex;
        }
        this->tail += oldSize;
    }
}
