#include "dsVECTOR.h"
#include "../ssl/sslSTACK_DATA.h"

// dsVECTOR<sslSTACK_DATA,8>::Reserve @ 0x82AF2678
template<>
void dsVECTOR<sslSTACK_DATA, 8>::Reserve(int size)
{
    if (size > this->allocated)
        this->Realloc(size);
}
