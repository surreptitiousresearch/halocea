#include <string.h>
#include "../headers/ws/ds/dsDEQUE.h"
#include "../headers/ws/dbg/STRONG_ASSERT2_HELPER.h"
#include "../headers/hcex/haloPLAYER_ACTIONS_ENV.h"

extern int IGNORE_STRONG_ASSERT;   // ?IGNORE_STRONG_ASSERT@@3HA

// dsDEQUE<haloPLAYER_ACTIONS_ENV>::Reconstruct(int oldSize) @ 0x823C01C8
// After the backing vector grew from `oldSize` slots, relocate whichever wrapped ring run is
// smaller into the freshly added tail space so the queue is contiguous-ordered again.
template<>
void dsDEQUE<haloPLAYER_ACTIONS_ENV>::Reconstruct(int oldSize)
{
    if (this->length > 0 && this->tail < this->head)
    {
        if (this->tail + 1 >= oldSize - this->head)
        {
            // Move the head run down into the newly grown tail region.
            // (&Back())[-oldSize+1] — pointer arithmetic off the last-live-element reference.
            haloPLAYER_ACTIONS_ENV *dst = &this->data.Back() - oldSize + 1;
            for (int i = this->head; i < oldSize; ++i)
            {
                haloPLAYER_ACTIONS_ENV *src = &this->data[i];
                if (&dst[i])
                    memcpy(&dst[i], src, sizeof(haloPLAYER_ACTIONS_ENV));
            }
            this->head += this->data.nElem - oldSize;
        }
        else
        {
            if (!IGNORE_STRONG_ASSERT && this->data.nElem <= 0)
                STRONG_ASSERT2_HELPER::asserd<int>(
                    "nElem > 0",
                    "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp", 507,
                    "nElem", this->data.nElem);
            // Move the tail run up into the newly grown region.
            haloPLAYER_ACTIONS_ENV *dst = &this->data.pData[oldSize];
            for (int j = 0; j <= this->tail; ++j)
            {
                haloPLAYER_ACTIONS_ENV *src = &this->data[j];
                if (&dst[j])
                    memcpy(&dst[j], src, sizeof(haloPLAYER_ACTIONS_ENV));
            }
            this->tail += oldSize;
        }
    }
}
