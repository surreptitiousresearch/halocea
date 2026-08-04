#include "dsVECTOR.h"
#include "dsTSTRING.h"

struct dsCMP {}; // boundary — stateless ascending/equality comparator; the string compare is inlined below

// dsVECTOR<dsTSTRING<char>,8>::Find<dsCMP,dsTSTRING<char>> @ 0x825B2948
// Linear scan from startPos for the first element whose text equals `key`'s (two-way inlined
// strcmp, matching the dsCMP idiom used by InsertSorted<dsCMP>). Returns its index or -1.
template<>
template<>
int dsVECTOR<dsTSTRING<char>, 8>::Find<dsCMP, dsTSTRING<char>>(
        const dsTSTRING<char> &key, const dsCMP &, int startPos) const
{
    int nElem = this->nElem;
    if (startPos >= nElem)
        return -1;

    for (int index = startPos; ; ) {
        const char *elemChar = this->pData[index].pBuffer->str;
        const char *keyChar = key.pBuffer->str;
        int forwardDiff;
        do {
            int elemByte = (unsigned char)*elemChar;
            forwardDiff = elemByte - (unsigned char)*keyChar;
            if (elemByte == 0)
                break;
            ++elemChar;
            ++keyChar;
        } while (!forwardDiff);

        if (forwardDiff >= 0) {
            const char *elemChar2 = this->pData[index].pBuffer->str;
            const char *keyChar2 = key.pBuffer->str;
            int backwardDiff;
            do {
                int keyByte = (unsigned char)*keyChar2;
                backwardDiff = keyByte - (unsigned char)*elemChar2;
                if (keyByte == 0)
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
