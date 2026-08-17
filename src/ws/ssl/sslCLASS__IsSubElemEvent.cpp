/* ?IsSubElemEvent@sslCLASS@@QBAHH@Z @0x82A389B0 */
#include "sslCLASS.h"
#include "sslCLASS_ELEMENT.h"

// 0x82A389B0 — sslCLASS::IsSubElemEvent. True when sub-element `idx` is an event handler.
// The binary uses an inclusive `idx <= elements.nElem` bound (reproduced faithfully).
int sslCLASS::IsSubElemEvent(int idx) const
{
    return idx >= 0
        && idx <= elements.nElem
        && elements[idx].type == sslCLASS_ELEMENT::TYPE_EVENT;
}
