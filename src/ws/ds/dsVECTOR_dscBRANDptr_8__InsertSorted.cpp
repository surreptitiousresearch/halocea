#include "dsVECTOR.h"
#include "dsTSTRING.h"
#include "../dsc/dscBRAND.h"

// Stateless name comparator functor: extracts dscBRAND::name and compares it (case-insensitively)
// to the key string. Layout-less; its key extraction/compare is inlined below. (dsc subsystem.)
template<class T, int A, int B>
struct dsNAME_CMP; // boundary

extern "C" int stricmp(const char *a, const char *b); // boundary — case-insensitive CRT compare

// dsVECTOR<dscBRAND*,8>::InsertSorted<dsNAME_CMP<dscBRAND,0,0>> @ 0x82711C00
// Binary-search insert into a name-ascending brand-pointer vector. On a miss, splice `val` at the
// lower-bound index (via Insert) and return it. On a match, `dup` selects overwrite / ignore /
// insert-after (see INS_SORT); an unrecognized `dup` returns -1. The dsNAME_CMP comparator is
// stateless; its case-insensitive name compare is inlined below.
template<>
template<>
int dsVECTOR<dscBRAND *, 8>::InsertSorted<dsNAME_CMP<dscBRAND, 0, 0> >(
    dscBRAND *const &val, const dsNAME_CMP<dscBRAND, 0, 0> & /*cmp*/, INS_SORT dup)
{
    int lo = 0;
    int hi = this->nElem;
    int diff = 0;
    int mid = 0;
    while (true) {
        if (lo == hi) {
            this->Insert(lo, val, 1);
            return lo;
        }
        mid = (lo + hi) >> 1;
        diff = stricmp(this->pData[mid]->name.pBuffer->str, val->name.pBuffer->str);
        if (diff <= 0) {
            if (diff >= 0)
                break;             // exact match at mid
            lo = mid + 1;          // val sorts after mid
        } else {
            hi = mid;               // val sorts before mid
        }
    }

    int matchIndex = (lo + hi) >> 1; // == mid at the break
    if (dup == INS_DUP_REPLACE) {
        this->pData[mid] = val;
    } else if (dup == INS_DUP_IGNORE) {
        // keep the existing element
    } else if (dup == INS_DUP_RIGHT) {
        this->Insert(mid, val, 1);
    } else {
        return -1;
    }
    return matchIndex;
}
