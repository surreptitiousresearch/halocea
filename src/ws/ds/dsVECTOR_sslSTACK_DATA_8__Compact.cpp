#include "dsVECTOR.h"
#include "../ssl/sslSTACK_DATA.h"

// dsVECTOR<sslSTACK_DATA,8>::Compact @ 0x82AF2660
template<>
void dsVECTOR<sslSTACK_DATA, 8>::Compact()
{
    this->Realloc(this->nElem);
}
