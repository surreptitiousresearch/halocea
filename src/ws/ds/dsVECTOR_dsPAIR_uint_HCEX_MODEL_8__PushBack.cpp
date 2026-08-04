#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "../../headers/hcex/HCEX_MODEL.h"
#include <new>

// dsVECTOR<dsPAIR<unsigned int,HCEX_MODEL>,8>::PushBack @ 0x823E7190
// Append a copy of `value` and return a reference to the new last element. The pair's `key`
// (a plain unsigned int) is copied directly; its `val` (HCEX_MODEL) has a non-trivial copy
// constructor/destructor and is copy-constructed into place. When the vector must grow, only the
// HCEX_MODEL is buffered into a temporary before Realloc (the key is a scalar snapshot).
template<>
dsPAIR<unsigned int, HCEX_MODEL> &dsVECTOR<dsPAIR<unsigned int, HCEX_MODEL>, 8>::PushBack(
        const dsPAIR<unsigned int, HCEX_MODEL> &value)
{
    int nElem = this->nElem;
    if (this->allocated >= nElem + 1) {
        // Spare capacity: write the key and copy-construct the model in place.
        dsPAIR<unsigned int, HCEX_MODEL> *slot = &this->pData[nElem];
        slot->key = value.key;
        new (&slot->val) HCEX_MODEL(value.val);
    } else {
        // Full: snapshot the key and buffer the model (it may alias pData) before reallocating.
        unsigned int key = value.key;
        HCEX_MODEL model(value.val);
        int newCapacity = 2 * this->allocated;
        if (newCapacity <= this->nElem + 1)
            newCapacity = this->nElem + 1;
        if (newCapacity > this->allocated)
            this->Realloc(newCapacity);
        dsPAIR<unsigned int, HCEX_MODEL> *slot = &this->pData[this->nElem];
        slot->key = key;
        new (&slot->val) HCEX_MODEL(model);
        // model destroyed here (~HCEX_MODEL inlined).
    }
    ++this->nElem;
    return this->Back();
}
