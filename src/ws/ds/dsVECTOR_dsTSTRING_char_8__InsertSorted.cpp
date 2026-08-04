#include "dsVECTOR.h"
#include "dsTSTRING.h"

struct dsCMP {}; // boundary — stateless ascending comparator; the string compare is inlined below

// dsVECTOR<dsTSTRING<char>,8>::InsertSorted<dsCMP> @ 0x823DFE98
// Binary-search insert into a vector kept ascending by lexical string order. On a miss, splice `val`
// at the lower-bound index and return it. On a match, `dup` selects the duplicate policy
// (replace / ignore / duplicate-right; anything else returns -1). The dsCMP compare is inlined as a
// byte-wise strcmp over the buffer characters.
template<>
template<>
int dsVECTOR<dsTSTRING<char>, 8>::InsertSorted<dsCMP>(
        const dsTSTRING<char> &val, const dsCMP &, INS_SORT dup)
{
    int hi = this->nElem;
    int lo = 0;
    if (!hi) {
        this->Insert(lo, &val, 1);
        return lo;
    }

    dsTSTRING<char> *pData = this->pData;
    while (true) {
        int mid = (hi + lo) >> 1;

        // strcmp(mid, val): stop at the first NUL or differing byte.
        const char *midChar = pData[mid].pBuffer->str;
        const char *valChar = val.pBuffer->str;
        int forwardDiff;
        do {
            int midByte = (unsigned char)*midChar;
            forwardDiff = midByte - (unsigned char)*valChar;
            if (midByte == 0)
                break;
            ++midChar;
            ++valChar;
        } while (!forwardDiff);

        if (forwardDiff < 0) {
            lo = mid + 1;
        } else {
            // strcmp(val, mid): decide whether val sorts strictly before mid (raise hi) or is equal.
            const char *valChar2 = pData[mid].pBuffer->str;
            const char *midChar2 = val.pBuffer->str;
            unsigned int reverseDiff;
            do {
                int valByte = (unsigned char)*midChar2;
                reverseDiff = valByte - (unsigned char)*valChar2;
                if (valByte == 0)
                    break;
                ++midChar2;
                ++valChar2;
            } while (!reverseDiff);
            if (reverseDiff >> 31 != 1)
                break; // equal match at mid
            hi = mid;
        }

        if (lo == hi) {
            this->Insert(lo, &val, 1);
            return lo;
        }
    }

    int matchIndex = (hi + lo) >> 1;
    if (dup == INS_DUP_REPLACE) {
        pData[matchIndex] = val; // dsTSTRING<char>::operator= (share buffer)
        return matchIndex;
    }
    if (dup == INS_DUP_IGNORE)
        return matchIndex;
    if (dup != INS_DUP_RIGHT)
        return -1;
    this->Insert(matchIndex, &val, 1);
    return matchIndex;
}
