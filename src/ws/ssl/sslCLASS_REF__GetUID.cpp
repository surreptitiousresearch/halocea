/* ?GetUID@sslCLASS_REF@@QBAHXZ @0x82523200 */
#include "sslCLASS_REF.h"
#include "sslCLASS.h"
// 0x82523200  sslCLASS_REF::GetUID

int sslCLASS_REF::GetUID() const
{
    if ( pClass )
        return pClass->uid;
    return -1;
}
