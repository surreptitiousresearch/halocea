#include "sslCLASS.h"

// 0x82A378C0 — sslCLASS::IsSubClassOf. Walk the parent chain; true if `pClass` is this class or
// any ancestor. const (QBA).
int sslCLASS::IsSubClassOf(sslCLASS *pClass) const
{
    if (this == pClass)
        return 1;

    const sslCLASS *current = this;
    for (;;)
    {
        sslCLASS *parent = current->refParent.pClass;
        if (!parent)
            break;
        current = parent;
        if (parent == pClass)
            return 1;
    }
    return 0;
}
