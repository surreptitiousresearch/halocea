// dsVECTOR<SSL_EXPORTER_haloENGINE_CONTROL::SSLExportEntry, 8>::~dsVECTOR() @ 0x823C5708
//
// Destroy the SSL export table: run each live entry's dtor (releases its sslDecl buffer), then free
// the backing store. Reversed per-instantiation.
#include "../headers/hcex/ssl_exporter/SSLExportEntry.h"
#include "../headers/ws/ds/dsVECTOR.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc free (unmangled C export)

template<>
dsVECTOR<SSL_EXPORTER_haloENGINE_CONTROL::SSLExportEntry, 8>::~dsVECTOR()
{
    for (int i = 0; i < this->nElem; ++i)
        this->pData[i].~SSLExportEntry();
    dlFree(this->pData);
}
