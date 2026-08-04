#include "dsVECTOR.h"
#include "../prop/instCONSTRUCTOR_NODE_RELATIONS.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// boundary — global placement copy-construct helper: New<T>(slot, src) === new (slot) T(*src).
#include <new>
template<class T> static T *New(T *slot, const T *src) { return ::new (static_cast<void *>(slot)) T(*src); }

// dsVECTOR<instCONSTRUCTOR_NODE_RELATIONS::RELATION,8>::PushBack @ 0x826E2370
// Append a copy of `val` at the tail, growing the backing store first when there is no spare
// capacity, and return a reference to the newly stored element. When growing, first builds a
// local RELATION copy (bumping the parent/child string refcounts, copying the transform floats)
// in case `val` aliases storage about to be reallocated, then releases the temporary's string
// references after the real slot is constructed from it.
template<>
instCONSTRUCTOR_NODE_RELATIONS::RELATION &dsVECTOR<instCONSTRUCTOR_NODE_RELATIONS::RELATION, 8>::PushBack(
        const instCONSTRUCTOR_NODE_RELATIONS::RELATION &val)
{
    if (this->allocated >= this->nElem + 1) {
        New<instCONSTRUCTOR_NODE_RELATIONS::RELATION>(&this->pData[this->nElem], &val);
    } else {
        dsTSTRING_BUF_HEADER<char> *parentBuffer = val.parent.pBuffer;
        dsTSTRING_BUF_HEADER<char> *childBuffer = val.child.pBuffer;
        instCONSTRUCTOR_NODE_RELATIONS::RELATION saved;
        saved.parent.pBuffer = parentBuffer;
        ++parentBuffer->refCount;
        saved.child.pBuffer = childBuffer;
        ++childBuffer->refCount;
        saved.translation = val.translation;
        saved.rotation = val.rotation;
        saved.scale = val.scale;

        int newCapacity = 2 * this->allocated;
        if (newCapacity <= this->nElem + 1)
            newCapacity = this->nElem + 1;
        if (newCapacity > this->allocated)
            this->Realloc(newCapacity);

        New<instCONSTRUCTOR_NODE_RELATIONS::RELATION>(&this->pData[this->nElem], &saved);

        if (childBuffer->refCount-- == 1)
            dlFree(childBuffer);
        if (parentBuffer->refCount-- == 1)
            dlFree(parentBuffer);
    }
    ++this->nElem;
    return this->Back();
}
