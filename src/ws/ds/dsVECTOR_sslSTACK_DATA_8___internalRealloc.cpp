#include "dsVECTOR.h"
#include "../ssl/sslSTACK_DATA.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<sslSTACK_DATA,8>::_internalRealloc @ 0x82AF0498
// sslSTACK_DATA is 16 bytes; trivially relocatable at the byte level (any owned ref-counts live on
// the referenced object/class, not on the stack-slot struct itself) — bare dlRealloc.
template<>
void dsVECTOR<sslSTACK_DATA, 8>::_internalRealloc(int size)
{
    this->pData = (sslSTACK_DATA *)dlRealloc(this->pData, sizeof(sslSTACK_DATA) * size, this->__cl.file, this->__cl.line);
}
