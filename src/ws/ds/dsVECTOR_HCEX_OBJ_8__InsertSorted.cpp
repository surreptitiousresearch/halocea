#include "dsVECTOR.h"
#include "../../headers/hcex/HCEX_OBJ.h"

struct HCEX_OBJ_CMP {}; // boundary — stateless comparator ordering HCEX_OBJ by its `id`

// dsVECTOR<HCEX_OBJ,8>::InsertSorted<HCEX_OBJ_CMP> @ 0x823EF058
// Binary-search insert into a vector kept ascending by HCEX_OBJ::id. On a miss, splice `val` at the
// lower-bound index and return it. On a match at `mid`, `dup` selects the duplicate policy
// (replace / ignore / duplicate-right; anything else returns -1).
template<>
template<>
int dsVECTOR<HCEX_OBJ, 8>::InsertSorted<HCEX_OBJ_CMP>(
        const HCEX_OBJ &val, const HCEX_OBJ_CMP &, INS_SORT dup)
{
    int hi = this->nElem;
    int lo = 0;
    if (!hi) {
        this->Insert(lo, &val, 1);
        return lo;
    }

    const HCEX_OBJ *pData = this->pData;
    int id = val.id;
    int mid;
    while (true) {
        mid = (hi + lo) >> 1;
        int midId = pData[mid].id;
        if (midId < id) {
            lo = mid + 1;
        } else if (id >= midId) { // midId >= id and id >= midId => equal match
            break;
        } else {
            hi = mid;
        }
        if (lo == hi) {
            this->Insert(lo, &val, 1);
            return lo;
        }
    }

    // Duplicate id already present at `mid`.
    if (dup == INS_DUP_REPLACE) {
        this->pData[mid] = val; // HCEX_OBJ::operator= (memberwise copy-assign)
    } else if (dup == INS_DUP_IGNORE) {
        // Keep the existing element.
    } else if (dup == INS_DUP_RIGHT) {
        this->Insert((hi + lo) >> 1, &val, 1);
    } else {
        return -1;
    }
    return mid;
}
