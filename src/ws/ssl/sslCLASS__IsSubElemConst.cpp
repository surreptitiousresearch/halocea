/* ?IsSubElemConst@sslCLASS@@QBAHH@Z @0x82A38AE0 */
#include "sslCLASS.h"
#include "sslCLASS_ELEMENT.h"

// 0x82A38AE0 — sslCLASS::IsSubElemConst. True when sub-element `idx` is a constant / base variable.
// The binary uses an inclusive `idx <= elements.nElem` bound (reproduced faithfully).
int sslCLASS::IsSubElemConst(int idx) const
{
    return idx >= 0
        && idx <= elements.nElem
        && elements[idx].type == sslCLASS_ELEMENT::TYPE_CONST;
}
