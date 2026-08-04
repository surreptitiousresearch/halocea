#include "dscDESC_FAMILY.h"
#include "dscBRAND.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsTSTRING_BUF_HEADER.h"

// dscDESC_FAMILY::IsDerivedFrom(const dscBRAND*, const dsTSTRING<char>&) @ 0x8270F5C8 (static)
// Walk `brand`'s parent chain, returning true if any ancestor's name equals `nameParent`.
// Each step first checks buffer-pointer identity (dsTSTRING's copy-on-write sharing means two
// equal strings very often share the same buffer), then falls back to a length + byte-for-byte
// compare against the target buffer's characters.
bool dscDESC_FAMILY::IsDerivedFrom(const dscBRAND *brand, const dsTSTRING<char> &nameParent)
{
    dscBRAND *ancestor = brand->pParent;
    if (!ancestor)
        return false;

    dsTSTRING_BUF_HEADER<char> *targetBuffer = nameParent.pBuffer;

    while (true) {
        dsTSTRING_BUF_HEADER<char> *ancestorBuffer = ancestor->name.pBuffer;
        bool namesMatch;

        if (ancestorBuffer == targetBuffer) {
            namesMatch = true;
        } else if (ancestorBuffer->strLen == targetBuffer->strLen) {
            int strLen = ancestorBuffer->strLen;
            if (strLen) {
                const char *targetChar = targetBuffer->str;
                const unsigned char *ancestorChar = (const unsigned char *)ancestorBuffer->str;
                const unsigned char *ancestorEnd = ancestorChar + strLen;
                int diff = 0;
                do {
                    diff = *ancestorChar - (unsigned char)*targetChar;
                    if (diff)
                        break;
                    ++ancestorChar;
                    ++targetChar;
                } while (ancestorChar != ancestorEnd);
                namesMatch = (diff == 0);
            } else {
                namesMatch = true;
            }
        } else {
            namesMatch = false;
        }

        if (namesMatch)
            break;

        ancestor = ancestor->pParent;
        if (!ancestor)
            return false;
    }

    return true;
}
