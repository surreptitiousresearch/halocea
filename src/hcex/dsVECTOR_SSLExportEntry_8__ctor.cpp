#include "../headers/hcex/ssl_exporter/SSLExportEntry.h"
#include "../headers/ws/ds/dsVECTOR.h"

// dsVECTOR<SSL_EXPORTER_haloENGINE_CONTROL::SSLExportEntry, 8>::dsVECTOR(const apCL &cl) @ 0x823C1C00
//
// Empty-vector ctor for SSL_EXPORTER_haloENGINE_CONTROL::GetExportMap()'s static export table:
// null storage, zero count/capacity, adopting the allocation call-site cookie `cl` by value.
// DEVIATION: the PPC decompiler renders the apCL member copy as two field writes (__cl.file = this,
// __cl.line = cl); that is an artifact of it not modeling the by-value apCL copy. Real: `__cl = cl`.
template<>
dsVECTOR<SSL_EXPORTER_haloENGINE_CONTROL::SSLExportEntry, 8>::dsVECTOR(const apCL &cl)
{
    this->pData = nullptr;
    this->nElem = 0;
    this->allocated = 0;
    this->__cl = cl;
}
