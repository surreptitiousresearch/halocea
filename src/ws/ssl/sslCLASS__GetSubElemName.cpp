/* ?GetSubElemName@sslCLASS@@QBA?AVdsSTRID@@H@Z @0x82A38B40 */
#include "sslCLASS.h"
#include "sslCLASS_ELEMENT.h"
#include "../ds/dsSTRID.h"

// 0x82A38B40 — sslCLASS::GetSubElemName. Returns the name of sub-element `idx` by value (sret).
// The decompiler swaps this/result (r3 = return slot dsSTRID*, r4 = this sslCLASS*): out of range
// yields a null dsSTRID, otherwise the element's name id. Inclusive `idx > elements.nElem` reject
// bound reproduced faithfully.
dsSTRID sslCLASS::GetSubElemName(int idx) const
{
    if (idx < 0 || idx > elements.nElem)
        return dsSTRID();
    return elements[idx].name;
}
