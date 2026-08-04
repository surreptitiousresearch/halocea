#include <string.h>
#include "../headers/havok/hkArray.h"

extern void hkArrayUtil__reserve(void *array, int numElem, int sizeElem); /* hkArrayUtil::_reserve */

/* hkArray<unsigned short>::spliceInto @ 0x83F97EA0
   Replace `numToRemove` elements starting at `index` with `numToInsert` elements
   copied from `src`, growing the backing store if the result would not fit.
   DEVIATION: the binary open-codes the two tail moves as direction-specific
   element loops (forward when shrinking, backward when growing) and the insert
   as a backward loop; both are exactly memmove/memcpy and expressed as such. */
void hkArrayUnsignedShort_spliceInto(hkArray<unsigned short> *self, int index, int numToRemove,
                                     const unsigned short *src, int numToInsert)
{
    int size = self->m_size;
    int capWord = self->m_capacityAndFlags;
    int newSize = size - numToRemove + numToInsert;
    int tailCount = size - index - numToRemove;

    if ((capWord & 0x3FFFFFFF) < newSize)
    {
        int grow = 2 * (capWord & 0x3FFFFFFF);
        if (newSize >= grow)
            grow = newSize;
        hkArrayUtil__reserve(&self->m_data, grow, 2);
    }

    if (tailCount > 0)
        memmove(&self->m_data[index + numToInsert], &self->m_data[index + numToRemove],
                (size_t)tailCount * sizeof(unsigned short));

    if (numToInsert > 0)
        memcpy(&self->m_data[index], src, (size_t)numToInsert * sizeof(unsigned short));

    self->m_size = newSize;
}
