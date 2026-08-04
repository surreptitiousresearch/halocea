#include "../headers/hcex/ssl_exporter/SSLExportEntry.h"
#include "../headers/ws/ds/dsTSTRING.h"
#include "../headers/ws/ds/dsTSTRING_BUF_HEADER.h"

// SSL_EXPORTER_haloENGINE_CONTROL::SSLExportEntry::SSLExportEntry(cb, id, sslDecl) @ 0x823C4BF0
// Full construct: adopt the callback and id-slot verbatim and share `_sslDecl`'s ref-counted buffer.
SSL_EXPORTER_haloENGINE_CONTROL::SSLExportEntry::SSLExportEntry(
    void (__fastcall *_cb)(sslOBJ_REF self, int argc, dsDATA *argv, dsDATA *retVal, sslOBJ_REF caller),
    int *_id,
    const dsTSTRING<char> *_sslDecl)
{
    this->cb = _cb;
    this->id = _id;
    this->sslDecl.pBuffer = nullptr;
    this->sslDecl.pBuffer = _sslDecl->pBuffer;
    ++this->sslDecl.pBuffer->refCount;
}
