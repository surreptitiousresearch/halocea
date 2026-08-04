#include "psSECTION.h"
#include "psSECTION_DATA.h"
// 0x8251A188 — drop this handle's reference to its section body (a no-op for an empty handle).

psSECTION::~psSECTION()
{
    if (this->pData)
        this->pData->Release();
}
