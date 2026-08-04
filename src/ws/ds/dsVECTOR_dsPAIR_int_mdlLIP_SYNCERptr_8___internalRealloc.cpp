#include "dsVECTOR.h"
#include "dsPAIR.h"

struct mdlLIP_SYNCER; // boundary — mdl subsystem lip-sync animator (element holds a pointer)

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<dsPAIR<int,mdlLIP_SYNCER*>,8>::_internalRealloc @ 0x823D04C8
// Element size 8 bytes (int key + pointer value).
template<>
void dsVECTOR<dsPAIR<int, mdlLIP_SYNCER *>, 8>::_internalRealloc(int size)
{
    this->pData = (dsPAIR<int, mdlLIP_SYNCER *> *)dlRealloc(this->pData, 8 * size, this->__cl.file, this->__cl.line);
}
