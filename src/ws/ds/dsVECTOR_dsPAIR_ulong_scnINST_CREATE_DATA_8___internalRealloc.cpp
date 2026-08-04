#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "../scn/scnINST_CREATE_DATA.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<dsPAIR<unsigned long,scnINST_CREATE_DATA>,8>::_internalRealloc @ 0x823D9570
// Element size 92 bytes (4-byte key + 88-byte scnINST_CREATE_DATA value).
template<>
void dsVECTOR<dsPAIR<unsigned long, scnINST_CREATE_DATA>, 8>::_internalRealloc(int size)
{
    this->pData = (dsPAIR<unsigned long, scnINST_CREATE_DATA> *)dlRealloc(this->pData, 92 * size, this->__cl.file, this->__cl.line);
}
