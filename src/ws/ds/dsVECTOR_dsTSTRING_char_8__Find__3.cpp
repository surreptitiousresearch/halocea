#include "dsVECTOR.h"
#include "dsTSTRING.h"

struct dsCMP {}; // boundary — stateless ascending/equality comparator; the string compare is inlined below

// dsVECTOR<dsTSTRING<char>,8>::Find<dsCMP,char*> @ 0x829817D8
// Linear scan from startPos for the first element whose text equals the raw NUL-terminated `*key`
// (two-way inlined strcmp). Returns its index or -1.
template<>
template<>
int dsVECTOR<dsTSTRING<char>, 8>::Find<dsCMP, char *>(
        char *const &key, const dsCMP &, int startPos) const
{
    int nElem = this->nElem;
    if (startPos >= nElem)
        return -1;

    for (int index = startPos; ; ) {
        const unsigned char *elemChar = (const unsigned char *)this->pData[index].pBuffer->str;
        const unsigned char *keyChar = (const unsigned char *)key;
        int forwardDiff;
        do {
            forwardDiff = *elemChar - *keyChar;
            if (*elemChar == 0)
                break;
            ++elemChar;
            ++keyChar;
        } while (!forwardDiff);

        if (forwardDiff >= 0) {
            const unsigned char *elemChar2 = (const unsigned char *)this->pData[index].pBuffer->str;
            const unsigned char *keyChar2 = (const unsigned char *)key;
            int backwardDiff;
            do {
                backwardDiff = *keyChar2 - *elemChar2;
                if (*keyChar2 == 0)
                    break;
                ++keyChar2;
                ++elemChar2;
            } while (!backwardDiff);
            if (backwardDiff >= 0)
                return index; // equal match
        }

        if (++index == nElem)
            return -1;
    }
}
