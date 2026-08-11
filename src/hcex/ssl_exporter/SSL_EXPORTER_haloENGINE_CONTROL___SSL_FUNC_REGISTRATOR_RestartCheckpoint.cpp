#include <cstddef>
#include "../../headers/hcex/ssl_exporter/SSLExportEntry.h"

// @0x823CBB10 — pushes the SSLExportEntry describing haloENGINE_CONTROL::RestartCheckpoint. `id`
// is the byte offset of the paired `sslFuncId_RestartCheckpoint` slot (@ 0x20).
SSL_EXPORTER_haloENGINE_CONTROL::_SSL_FUNC_REGISTRATOR_RestartCheckpoint::_SSL_FUNC_REGISTRATOR_RestartCheckpoint()
{
    dsTSTRING<char> decl;
    decl.UnsafeInit("RestartCheckpoint()", -1, 0);

    SSL_EXPORTER_haloENGINE_CONTROL::SSLExportEntry entry(
        SSL_EXPORTER_haloENGINE_CONTROL::cbRestartCheckpoint,
        (int *)offsetof(SSL_EXPORTER_haloENGINE_CONTROL, sslFuncId_RestartCheckpoint),
        &decl);

    SSL_EXPORTER_haloENGINE_CONTROL::GetExportMap()->PushBack(entry);
}
