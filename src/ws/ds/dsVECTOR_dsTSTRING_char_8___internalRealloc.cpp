#include "dsVECTOR.h"
#include "dsTSTRING.h"

extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line); // boundary

// dsVECTOR<dsTSTRING<char>,8>::_internalRealloc @ 0x823B3568
// dsTSTRING<char> is a single pointer (4 bytes) and trivially relocatable, so this is a bare
// dlRealloc with no per-element teardown.
template<>
void dsVECTOR<dsTSTRING<char>, 8>::_internalRealloc(int size)
{
    this->pData = (dsTSTRING<char> *)dlRealloc(this->pData, 4 * size, this->__cl.file, this->__cl.line);
}
