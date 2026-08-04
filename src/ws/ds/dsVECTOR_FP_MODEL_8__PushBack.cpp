#include "dsVECTOR.h"
#include "../../headers/fp_model_vector.h"

// dsVECTOR<FP_MODEL,8>::PushBack @ 0x823E25C0
// Append `val` to the tail (a trivially relocatable 24-byte POD record), growing the backing
// store first when there is no spare capacity, and return a reference to the newly stored
// element. DEVIATION: the decompile shows the whole-struct copy unrolled as six word-sized moves
// walking from an odd `&val[-1].isHiddenCam1 + 4*i` base (a codegen artifact of copying the
// struct via its trailing byte-field alignment) — reconstructed here as the equivalent plain
// struct assignment.
template<>
FP_MODEL &dsVECTOR<FP_MODEL, 8>::PushBack(const FP_MODEL &val)
{
    if (this->allocated >= this->nElem + 1) {
        this->pData[this->nElem] = val;
    } else {
        FP_MODEL saved = val; // save before Realloc may invalidate `val`
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
