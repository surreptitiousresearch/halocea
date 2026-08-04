#include "sslOBJ_REF.h"
#include "sslOBJECT.h"
// 0x82523388  sslOBJ_REF::GetUID

int sslOBJ_REF::GetUID() const
{
    if ( pObject )
        return pObject->uid;
    return -1;
}
