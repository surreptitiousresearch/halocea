#include "psSECTION_INTERFACE.h"
// 0x82515EB0 — true when the wrapped section handle carries no body.

int psSECTION_INTERFACE::IsNull() const
{
    return this->pData == nullptr;
}
