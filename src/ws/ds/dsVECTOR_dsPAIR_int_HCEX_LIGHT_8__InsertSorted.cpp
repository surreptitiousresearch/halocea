#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "../../headers/hcex/hcex_light.h"

struct dsCMP {}; // boundary — stateless ascending comparator; the key compare is inlined below

// dsVECTOR<dsPAIR<int,HCEX_LIGHT>,8>::InsertSorted<dsCMP> @ 0x823E40B0
// Binary-search insert into a vector kept ascending by the pair's int `key`. On a miss, splice
// `val` at the lower-bound index and return it. On a match at `mid`, `dup` selects the duplicate
// policy (replace / ignore / duplicate-right; anything else returns -1).
template<>
template<>
int dsVECTOR<dsPAIR<int, HCEX_LIGHT>, 8>::InsertSorted<dsCMP>(
        const dsPAIR<int, HCEX_LIGHT> &val, const dsCMP &, INS_SORT dup)
{
    int hi = this->nElem;
    int lo = 0;
    if (!hi) {
        this->Insert(lo, &val, 1);
        return lo;
    }

    const dsPAIR<int, HCEX_LIGHT> *pData = this->pData;
    int key = val.key;
    int mid;
    while (true) {
        mid = (hi + lo) >> 1;
        int midKey = pData[mid].key;
        if (midKey < key) {
            lo = mid + 1;
        } else if (key >= midKey) { // midKey >= key and key >= midKey => equal match
            break;
        } else {
            hi = mid;
        }
        if (lo == hi) {
            this->Insert(lo, &val, 1);
            return lo;
        }
    }

    // Duplicate key already present at `mid`.
    if (dup == INS_DUP_REPLACE) {
        this->pData[mid] = val; // dsPAIR<int,HCEX_LIGHT>::operator= (memberwise copy-assign)
    } else if (dup == INS_DUP_IGNORE) {
        // Keep the existing element.
    } else if (dup == INS_DUP_RIGHT) {
        this->Insert((hi + lo) >> 1, &val, 1);
    } else {
        return -1;
    }
    return mid;
}
