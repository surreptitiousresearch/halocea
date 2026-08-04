#include "../../headers/ws/ai/aiDOM_SPAWN.h"
#include "../../headers/ws/ds/dsCONST_ARRAY.h"
#include <string.h>

// aiDOM_SPAWN::GetNameBHV @ 0x83264858  ?GetNameBHV@aiDOM_SPAWN@@UAAHPADH@Z (virtual)
// Pick one of the desc's up-to-20 behaviour names (exclusive RNG) and copy it into `buf`.
// &desc[38] is the dsCONST_ARRAY<dsTSTRING<char>,20> of behaviour names; its first word
// (desc[38].__vftable slot) is the populated count.
int aiDOM_SPAWN::GetNameBHV(char *buf, unsigned int size)
{
    if ((int)this->spDesc.pointee[38].__vftable <= 0)
        return 0;

    int idx = this->rxBhv.Get();
    // dsCONST_ARRAY::operator[] returns T& (dsTSTRING<char>&), not a pointer
    dsTSTRING<char> &name =
        (*(dsCONST_ARRAY<dsTSTRING<char>, 20> *)&this->spDesc.pointee[38])[idx];
    if (!name.pBuffer->strLen)
        return 0;

    strncpy(buf, name.pBuffer->str, size);
    return 1;
}
