#include "dsVECTOR.h"
#include "dsTSTRING.h"
#include "../dsc/dscBRAND.h"

// Stateless name comparator functor: extracts dscBRAND::name and compares it (case-insensitively)
// to the key string. Layout-less; its key extraction/compare is inlined below. (dsc subsystem.)
template<class T, int A, int B>
struct dsNAME_CMP; // boundary

extern "C" int stricmp(const char *a, const char *b); // boundary — case-insensitive CRT compare

// dsVECTOR<dscBRAND*,8>::FindSorted<dsNAME_CMP<dscBRAND,0,0>,dsTSTRING<char>> @ 0x8270F858
// Binary search a name-ascending brand-pointer vector for the brand whose name equals `key`
// (case-insensitive compare of dscBRAND::name against the key string); returns its index, or
// -1 if absent. const method (QBA mangle). The dsNAME_CMP comparator is stateless.
template<>
template<>
int dsVECTOR<dscBRAND *, 8>::FindSorted<dsNAME_CMP<dscBRAND, 0, 0>, dsTSTRING<char> >(
    const dsTSTRING<char> &key, const dsNAME_CMP<dscBRAND, 0, 0> & /*cmp*/) const
{
    int hi = this->nElem;
    int lo = 0;
    if (hi) {
        do {
            int mid = (hi + lo) >> 1;
            int diff = stricmp(this->pData[mid]->name.pBuffer->str, key.pBuffer->str);
            if (diff <= 0) {
                if (diff >= 0)
                    return mid;
                lo = mid + 1;
            } else {
                hi = mid;
            }
        } while (lo != hi);
    }
    return -1;
}
