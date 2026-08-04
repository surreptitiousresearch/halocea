#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "dsTSTRING.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free

// New<T>(dest, src) — placement-construct a copy of *src at *dest. Reversed sibling helper used
// throughout dsVECTOR; treated as a boundary here (memberwise copy-construct: key + bump val's
// buffer refcount).
template<class T> void New(T *dest, const T *src);

// dsVECTOR<dsPAIR<int,dsTSTRING<char>>,8>::PushBack @ 0x82AA88C0
// Append a copy of `val` and return a reference to the new last element. When the vector must grow,
// `val` (which may alias the backing store Realloc frees) is first buffered into a temporary pair
// (taking its own reference on the dsTSTRING buffer) before Realloc; the temporary's reference is
// released at the end.
template<>
dsPAIR<int, dsTSTRING<char> > &dsVECTOR<dsPAIR<int, dsTSTRING<char> >, 8>::PushBack(
        const dsPAIR<int, dsTSTRING<char> > &val)
{
    if (this->allocated >= this->nElem + 1) {
        New<dsPAIR<int, dsTSTRING<char> > >(&this->pData[this->nElem], &val);
    } else {
        dsPAIR<int, dsTSTRING<char> > temp;
        temp.key = val.key;
        dsTSTRING_BUF_HEADER<char> *buffer = val.val.pBuffer;
        temp.val.pBuffer = buffer;
        ++buffer->refCount;

        int newCapacity = this->nElem + 1;
        if (2 * this->allocated > newCapacity)
            newCapacity = 2 * this->allocated;
        if (newCapacity > this->allocated)
            this->Realloc(newCapacity);

        New<dsPAIR<int, dsTSTRING<char> > >(&this->pData[this->nElem], &temp);

        // Release the temporary's own reference (destructor of the buffered pair).
        if (temp.val.pBuffer->refCount-- == 1)
            dlFree(temp.val.pBuffer);
    }
    ++this->nElem;
    return this->Back();
}
