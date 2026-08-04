#include "dsVECTOR.h"

// boundary — ds::MAP<objOBJ*,dsVECTOR<objOBJ*,8>,ds::HASH,ds::CMP,dsNODE_CACHE_ALLOCATOR>::DUMMY,
// a hash-map node record keying object pointers to per-key object vectors. Opaque here;
// the element is a pointer. Distinct tag avoids ODR collision with other MAP DUMMY records.
struct dsObjOBJVecMapDUMMY;

// dsVECTOR<dsObjOBJVecMapDUMMY *,8>::PushBack @ 0x82688720
// Append `val` to the tail, growing the backing store first when there is no spare capacity,
// and return a reference to the newly stored pointer slot.
template<>
dsObjOBJVecMapDUMMY *&dsVECTOR<dsObjOBJVecMapDUMMY *, 8>::PushBack(dsObjOBJVecMapDUMMY *const &val)
{
    if (this->allocated >= this->nElem + 1) {
        // spare capacity: store straight into the tail slot
        this->pData[this->nElem] = val;
    } else {
        // must grow: save `val` first in case it aliases storage about to be reallocated
        dsObjOBJVecMapDUMMY *saved = val;
        int newCapacity = 2 * this->allocated;
        if (newCapacity <= this->nElem + 1)
            newCapacity = this->nElem + 1;
        if (newCapacity > this->allocated)
            this->Realloc(newCapacity);
        this->pData[this->nElem] = saved;
    }
    ++this->nElem;
    return this->Back();
}
