#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "dsTSTRING.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<dsPAIR<int,dsTSTRING<char>>,8>::_internalRealloc @ 0x825CA230
// Element size 8 bytes (int key + dsTSTRING<char> pointer value).
template<>
void dsVECTOR<dsPAIR<int, dsTSTRING<char> >, 8>::_internalRealloc(int size)
{
    this->pData = (dsPAIR<int, dsTSTRING<char> > *)dlRealloc(this->pData, 8 * size, this->__cl.file, this->__cl.line);
}
