#include "dsVECTOR.h"
#include "dsTSTRING.h"
#include "../../apCL.h"

// dsVECTOR<dsTSTRING<char>,8>::dsVECTOR(int, const apCL&) @ 0x82BE5348
// Deviation: the decompiler fabricated a single "__int64 cl" parameter and read a fictitious
// SHIDWORD(cl) as the Resize() argument. Per the mangle (ctor takes int, apCL) and the disasm
// (`std r5, 0xC(r3)` stores only r5 — the 8-byte apCL passed packed in one GPR per the Xbox360
// ABI — into __cl; r4, the untouched int `size` argument, is left live in place and used directly
// by the trailing `bl Resize(int)`), the real signature/body is reconstructed below.
template<>
dsVECTOR<dsTSTRING<char>, 8>::dsVECTOR(int size, const apCL &cl)
{
    this->__cl = cl;
    this->pData = nullptr;
    this->nElem = 0;
    this->allocated = 0;
    this->Resize(size);
}
