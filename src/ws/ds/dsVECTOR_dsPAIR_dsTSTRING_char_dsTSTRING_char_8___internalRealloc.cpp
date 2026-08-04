#include "dsVECTOR.h"
#include "dsPAIR.h"
#include "dsTSTRING.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<dsPAIR<dsTSTRING<char>,dsTSTRING<char>>,8>::_internalRealloc @ 0x823D0448
// Element size 8 bytes (two dsTSTRING<char> pointer-handles).
template<>
void dsVECTOR<dsPAIR<dsTSTRING<char>, dsTSTRING<char> >, 8>::_internalRealloc(int size)
{
    this->pData = (dsPAIR<dsTSTRING<char>, dsTSTRING<char> > *)dlRealloc(this->pData, 8 * size, this->__cl.file, this->__cl.line);
}
