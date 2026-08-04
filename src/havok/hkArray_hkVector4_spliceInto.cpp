#include "../headers/havok/hkArray.h"
#include "../headers/havok/hkVector4.h"

extern void hkArrayUtil__reserve(void *array, int numElem, int sizeElem); /* hkArrayUtil::_reserve */
extern void *memmove(void *dst, const void *src, unsigned int n);

/* hkArray<hkVector4>::spliceInto — replace numToRemove elements at `index` with
   numToInsert elements copied from insertData, growing the storage when the
   resulting size exceeds capacity.
   DEVIATION: the original open-codes the tail shift as two VMX128 element-copy
   loops (forward when shrinking, backward when growing) plus the insert loop;
   reconstructed with memmove (which subsumes both overlap directions) and a
   straight element copy. Element size is sizeof(hkVector4) == 16. */
void hkArray_hkVector4_spliceInto(hkArray<hkVector4> *self, int index, int numToRemove,
                                  const hkVector4 *insertData, int numToInsert)
{
    int oldSize = self->m_size;
    int capacity = self->m_capacityAndFlags & 0x3FFFFFFF;
    int newSize = oldSize - numToRemove + numToInsert;
    int tailCount = oldSize - index - numToRemove;
    int i;

    if (capacity < newSize)
    {
        int grow = 2 * capacity;
        if (newSize >= grow)
            grow = newSize;
        hkArrayUtil__reserve(&self->m_data, grow, (int)sizeof(hkVector4));
    }

    if (tailCount > 0)
        memmove(&self->m_data[index + numToInsert],
                &self->m_data[index + numToRemove],
                tailCount * (int)sizeof(hkVector4));

    for (i = 0; i < numToInsert; ++i)
        self->m_data[index + i] = insertData[i];

    self->m_size = newSize;
}
