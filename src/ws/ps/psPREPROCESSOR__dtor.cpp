#include "psPREPROCESSOR.h"
// @0x82A2D200 — destroy the macro-name vector, release the result/errorDesc text buffers, and free
// the #if/#else stack's raw backing storage (a plain byte array with no per-element teardown).

extern "C" void dlFree(void *ptr); // boundary — debug allocator free

psPREPROCESSOR::~psPREPROCESSOR()
{
    macro.~dsVECTOR<dsTSTRING<char>, 8>();
    dlFree(stack.pData);
}
