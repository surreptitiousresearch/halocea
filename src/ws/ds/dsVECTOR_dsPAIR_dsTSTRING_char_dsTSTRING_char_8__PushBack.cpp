#include <new>
#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "dsTSTRING.h"

// dsPAIR<dsTSTRING<char>,dsTSTRING<char>> copy ctor / dtor (memberwise: bump/drop each string's
// buffer refcount). Reversed sibling; treated as a boundary here. // boundary
template<> dsPAIR<dsTSTRING<char>, dsTSTRING<char> >::dsPAIR(const dsPAIR<dsTSTRING<char>, dsTSTRING<char> > &other);
template<> dsPAIR<dsTSTRING<char>, dsTSTRING<char> >::~dsPAIR();

// dsVECTOR<dsPAIR<dsTSTRING<char>,dsTSTRING<char>>,8>::PushBack @ 0x82B05DE8
// Append a copy of `val` and return a reference to the new last element. When the vector must grow,
// `val` is first copy-constructed into a stack temporary (its own references taken) before Realloc
// (which may move/free the backing store); the new slot is then copy-constructed from that
// temporary, which is destroyed at the end.
template<>
dsPAIR<dsTSTRING<char>, dsTSTRING<char> > &dsVECTOR<dsPAIR<dsTSTRING<char>, dsTSTRING<char> >, 8>::PushBack(
        const dsPAIR<dsTSTRING<char>, dsTSTRING<char> > &val)
{
    if (this->allocated >= this->nElem + 1) {
        dsPAIR<dsTSTRING<char>, dsTSTRING<char> > *slot = &this->pData[this->nElem];
        if (slot)
            new (slot) dsPAIR<dsTSTRING<char>, dsTSTRING<char> >(val);
    } else {
        dsPAIR<dsTSTRING<char>, dsTSTRING<char> > temp(val);

        int newCapacity = this->nElem + 1;
        if (2 * this->allocated > newCapacity)
            newCapacity = 2 * this->allocated;
        if (newCapacity > this->allocated)
            this->Realloc(newCapacity);

        dsPAIR<dsTSTRING<char>, dsTSTRING<char> > *slot = &this->pData[this->nElem];
        if (slot)
            new (slot) dsPAIR<dsTSTRING<char>, dsTSTRING<char> >(temp);
        temp.~dsPAIR<dsTSTRING<char>, dsTSTRING<char> >();
    }
    ++this->nElem;
    return this->Back();
}
