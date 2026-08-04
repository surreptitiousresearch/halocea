#include "dsVECTOR.h"
#include "dsTSTRING.h"

struct dsCMP_EQ {}; // boundary — stateless single-pass equality comparator (strcmp, one direction only)

// dsVECTOR<dsTSTRING<char>,8>::Find<dsCMP_EQ,const char*> @ 0x82D9DCA8
// Linear scan from startPos for the first element whose text equals the raw NUL-terminated `*key`.
// Unlike the dsCMP overloads, dsCMP_EQ's compare is a single forward strcmp pass (no backward
// verification pass — equality is decided by the forward scan reaching '\0' with a zero diff).
template<>
template<>
int dsVECTOR<dsTSTRING<char>, 8>::Find<dsCMP_EQ, const char *>(
        const char *const &key, const dsCMP_EQ &, int startPos) const
{
    if (startPos >= this->nElem)
        return -1;

    for (int index = startPos; index != this->nElem; ++index) {
        const unsigned char *elemChar = (const unsigned char *)this->pData[index].pBuffer->str;
        const unsigned char *keyChar = (const unsigned char *)key;
        int diff;
        do {
            diff = *elemChar - *keyChar;
            if (*elemChar == 0)
                break;
            ++elemChar;
            ++keyChar;
        } while (!diff);
        if (!diff)
            return index;
    }
    return -1;
}
