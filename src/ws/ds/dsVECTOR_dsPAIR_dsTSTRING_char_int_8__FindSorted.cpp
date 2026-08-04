#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "dsTSTRING.h"

struct dsCMP {}; // boundary — stateless string comparator (compare inlined below)

// dsVECTOR<dsPAIR<dsTSTRING<char>,int>,8>::FindSorted<dsCMP,dsTSTRING<char>> @ 0x823B52E8
// Binary search of the object-name -> Blam-id table (hcexObjectsCine), kept ascending by string
// key, for the pair whose key equals `key`; returns its index or -1. The comparator is inlined as
// a byte-wise string compare, applied twice per probe (mid-vs-key to pick a half, then key-vs-mid
// to confirm equality).
template<>
template<>
int dsVECTOR<dsPAIR<dsTSTRING<char>, int>, 8>::FindSorted<dsCMP, dsTSTRING<char> >(
        const dsTSTRING<char> &key, const dsCMP &cmp) const
{
    (void)cmp;
    int hi = this->nElem;
    int lo = 0;
    if (hi) {
        const char *keyStr = key.pBuffer->str;
        const dsPAIR<dsTSTRING<char>, int> *elems = this->pData;
        do {
            int mid = (hi + lo) >> 1;

            // three-way compare elems[mid].key vs key
            const char *midChar = elems[mid].key.pBuffer->str;
            const char *keyChar = keyStr;
            int cmpMidKey;
            do {
                cmpMidKey = (unsigned char)*midChar - (unsigned char)*keyChar;
                if (*midChar == 0)
                    break;
                ++midChar;
                ++keyChar;
            } while (!cmpMidKey);

            if (cmpMidKey < 0) {
                lo = mid + 1; // mid.key < key — search the upper half
            } else {
                // mid.key >= key; confirm equality by comparing key vs mid.key
                const char *midChar2 = elems[mid].key.pBuffer->str;
                const char *keyChar2 = keyStr;
                unsigned int cmpKeyMid;
                do {
                    cmpKeyMid = (unsigned char)*keyChar2 - (unsigned char)*midChar2;
                    if (*keyChar2 == 0)
                        break;
                    ++keyChar2;
                    ++midChar2;
                } while (!cmpKeyMid);
                if (cmpKeyMid >> 31 != 1) // key >= mid.key and mid.key >= key ⇒ equal
                    return mid;
                hi = mid; // key < mid.key — search the lower half
            }
        } while (lo != hi);
    }
    return -1;
}
