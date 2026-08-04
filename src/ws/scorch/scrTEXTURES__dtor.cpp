// scrTEXTURES::~scrTEXTURES @0x82726F40 — release every non-null texture pointer (cube, hm,
// detNM, nm, dif — in that order) via txmTEXTURE::Release, but only while txmManager is alive.
#include "../../headers/ws/scorch/scrTEXTURES.h"
#include "../../headers/ws/txm/txmTEXTURE.h"

scrTEXTURES::~scrTEXTURES()
{
    if (cube.ptr && txmManager)  cube.ptr->Release();  // Release() is 0-arg (instance method)
    if (hm.ptr && txmManager)    hm.ptr->Release();
    if (detNM.ptr && txmManager) detNM.ptr->Release();
    if (nm.ptr && txmManager)    nm.ptr->Release();
    if (dif.ptr && txmManager)   dif.ptr->Release();
}
