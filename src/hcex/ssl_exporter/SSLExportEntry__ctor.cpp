#include "../../headers/hcex/ssl_exporter/SSLExportEntry.h"

// 0x823C4BF4 — full construct: adopt `_cb`/`_id` verbatim, then share `_sslDecl`'s buffer
// (bump its refcount). Used by RegisterEntry() and (semantically, though the raw decompile
// inlines the field assignment instead of calling this ctor) by each _SSL_FUNC_REGISTRATOR_* ctor.
SSL_EXPORTER_haloENGINE_CONTROL::SSLExportEntry::SSLExportEntry(
    void (__fastcall *_cb)(sslOBJ_REF self, int argc, dsDATA *argv, dsDATA *retVal, sslOBJ_REF caller),
    int *_id,
    const dsTSTRING<char> *_sslDecl)
{
    cb = _cb;
    id = _id;

    sslDecl.pBuffer = nullptr;
    dsTSTRING_BUF_HEADER<char> *sourceBuffer = _sslDecl->pBuffer;
    sslDecl.pBuffer = sourceBuffer;
    ++sourceBuffer->refCount;
}
