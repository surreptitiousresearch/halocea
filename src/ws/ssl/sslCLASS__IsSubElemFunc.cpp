#include "sslCLASS.h"
#include "sslCLASS_ELEMENT.h"

// 0x82A388E8 — sslCLASS::IsSubElemFunc. True when sub-element `idx` is a function (native callback
// or scripted). The binary uses an inclusive `idx > elements.nElem` reject test (reproduced faithfully).
int sslCLASS::IsSubElemFunc(int idx) const
{
    if (idx < 0 || idx > elements.nElem)
        return 0;
    unsigned char elemType = elements[idx].type;
    return elemType == sslCLASS_ELEMENT::TYPE_CB_FUNC
        || elemType == sslCLASS_ELEMENT::TYPE_SSL_FUNC;
}
