#include "dsVECTOR.h"
#include "../ssl/sslCLASS_REF.h"

// dsVECTOR<sslCLASS_REF,8>::Reserve @ 0x825D01A0
template<>
void dsVECTOR<sslCLASS_REF, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
