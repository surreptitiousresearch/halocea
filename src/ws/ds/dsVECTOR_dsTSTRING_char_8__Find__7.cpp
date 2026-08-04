#include "dsVECTOR.h"
#include "dsTSTRING.h"

struct fsmWILD_CARD_CMP {}; // boundary — stateless wildcard-pattern comparator

// dsVECTOR<dsTSTRING<char>,8>::Find<fsmWILD_CARD_CMP,dsTSTRING<char>> @ 0x82F5B098
// CAVEAT: fsmWILD_CARD_CMP's compare is inlined as key.Find(elementText, 0, 0) — i.e. the wildcard
// comparator is implemented via dsTSTRING<char>::Find (substring/pattern search), and the loop
// treats a ZERO return (match found at char offset 0) as "this element matches the pattern",
// continuing the scan on any nonzero (not-found, or found-but-not-at-offset-0) result. The exact
// wildcard semantics live inside dsTSTRING<char>::Find/IsStartWith, not reconstructed here; this
// mirrors the disasm's call shape exactly.
template<>
template<>
int dsVECTOR<dsTSTRING<char>, 8>::Find<fsmWILD_CARD_CMP, dsTSTRING<char>>(
        const dsTSTRING<char> &key, const fsmWILD_CARD_CMP &, int startPos) const
{
    int nElem = this->nElem;
    if (startPos >= nElem)
        return -1;

    int index = startPos;
    while (key.Find(this->pData[index].pBuffer->str, 0, 0) != 0) {
        if (++index == nElem)
            return -1;
    }
    return index;
}
