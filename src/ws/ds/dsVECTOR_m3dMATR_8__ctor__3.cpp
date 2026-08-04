#include "dsVECTOR.h"
#include "../m3d/m3dMATR.h"
#include "../apCL.h"

// dsVECTOR<m3dMATR,8>::dsVECTOR(int,apCL) @ 0x828623D0
// Deviation: decompiler packs `cl` into a 64-bit local and reads the initial size out of its
// high dword (SHIDWORD(cl)) — that is the ABI's overlapped-register packing of the (int size,
// apCL cl) argument pair; reconstructed as the two real separate parameters.
template<>
dsVECTOR<m3dMATR, 8>::dsVECTOR(int size, const apCL &cl)
{
    this->__cl = cl;
    this->pData = nullptr;
    this->nElem = 0;
    this->allocated = 0;
    this->Resize(size);
}
