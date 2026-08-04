#include "dsVECTOR.h"
#include "dsPARAM.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<dsPARAM,8>::_internalRealloc @ 0x823D00B0
// dsPARAM is 12 bytes ({dsSTRID id; dsDATA data;}); trivially relocatable, so a bare dlRealloc.
template<>
void dsVECTOR<dsPARAM, 8>::_internalRealloc(int size)
{
    this->pData = (dsPARAM *)dlRealloc(this->pData, sizeof(dsPARAM) * size, this->__cl.file, this->__cl.line);
}
