#include "dsVECTOR.h"
#include "dsTSTRING.h"
#include <string.h>

namespace snd { namespace SYSTEM_FMOD {
struct PRESET_NAME_CMP {}; // boundary — stateless case-insensitive equality comparator (snd subsystem)
} }

// dsVECTOR<dsTSTRING<char>,8>::Find<snd::SYSTEM_FMOD::PRESET_NAME_CMP,dsTSTRING<char>> @ 0x82E2C140
// Linear scan from startPos for the first element whose text case-insensitively equals `key`'s
// (stricmp). Returns its index or -1.
template<>
template<>
int dsVECTOR<dsTSTRING<char>, 8>::Find<snd::SYSTEM_FMOD::PRESET_NAME_CMP, dsTSTRING<char>>(
        const dsTSTRING<char> &key, const snd::SYSTEM_FMOD::PRESET_NAME_CMP &, int startPos) const
{
    int nElem = this->nElem;
    if (startPos >= nElem)
        return -1;

    int index = startPos;
    while (stricmp(this->pData[index].pBuffer->str, key.pBuffer->str) != 0) {
        if (++index == nElem)
            return -1;
    }
    return index;
}
