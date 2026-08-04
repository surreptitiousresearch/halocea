#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "dsTSTRING.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<dsPAIR<dsTSTRING<char>,int>,8>::_internalRealloc @ 0x823B3768
// Element size 8 bytes (dsTSTRING<char> pointer key + int value).
template<>
void dsVECTOR<dsPAIR<dsTSTRING<char>, int>, 8>::_internalRealloc(int size)
{
    this->pData = (dsPAIR<dsTSTRING<char>, int> *)dlRealloc(this->pData, 8 * size, this->__cl.file, this->__cl.line);
}
