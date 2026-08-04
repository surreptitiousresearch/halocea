#include "dsVECTOR.h"
#include "dsPARAM.h"

// dsVECTOR<dsPARAM,8>::Reserve @ 0x823E01B0
template<>
void dsVECTOR<dsPARAM, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
