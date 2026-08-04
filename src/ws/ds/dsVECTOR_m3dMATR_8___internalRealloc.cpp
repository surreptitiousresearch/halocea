#include "dsVECTOR.h"
#include "../m3d/m3dMATR.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<m3dMATR,8>::_internalRealloc @ 0x823D0178
// m3dMATR is 64 bytes (sizeof(m3dMATR) == 0x40); trivially relocatable, so a bare dlRealloc.
template<>
void dsVECTOR<m3dMATR, 8>::_internalRealloc(int size)
{
    this->pData = (m3dMATR *)dlRealloc(this->pData, sizeof(m3dMATR) * size, this->__cl.file, this->__cl.line);
}
