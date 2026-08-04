#include "dsVECTOR.h"
#include "dsPAIR.h"

struct HCEX_MODEL { char _opaque[72]; }; // boundary — hcex model record; only the pair key is read
struct dsCMP {};                         // boundary — key comparator (empty; comparison is inlined)

// dsVECTOR<dsPAIR<unsigned long,HCEX_MODEL>,8>::Find<dsCMP,unsigned long> @ 0x823D4140
// Linear scan from startPos for the first pair whose key equals `key`; -1 if none.
template<>
template<>
int dsVECTOR<dsPAIR<unsigned long, HCEX_MODEL>, 8>::Find<dsCMP, unsigned long>(
        const unsigned long &key, const dsCMP &cmp, int startPos) const
{
    (void)cmp; // comparator is empty; the key equality is emitted inline
    const int count = this->nElem;
    if (startPos >= count)
        return -1;

    const dsPAIR<unsigned long, HCEX_MODEL> *elems = this->pData;
    const unsigned int target = key;
    int foundIndex = startPos;
    for (int scanIndex = startPos; ; ++scanIndex) {
        unsigned int elemKey = elems[scanIndex].key;
        if (elemKey >= target && target >= elemKey) // equality via two unsigned compares
            break;
        if (++foundIndex == count)
            return -1;
    }
    return foundIndex;
}
