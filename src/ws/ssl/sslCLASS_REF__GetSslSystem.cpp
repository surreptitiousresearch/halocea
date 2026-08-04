#include "sslCLASS_REF.h"
#include "sslCLASS.h"
// 0x825231E8  sslCLASS_REF::GetSslSystem

sslSYSTEM *sslCLASS_REF::GetSslSystem() const
{
    if ( pClass )
        return pClass->pSslSystem;
    return nullptr;
}
