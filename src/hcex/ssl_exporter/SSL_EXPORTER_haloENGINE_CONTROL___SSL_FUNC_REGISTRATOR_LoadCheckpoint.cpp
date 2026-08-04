#include <cstddef>
#include "../../headers/hcex/ssl_exporter/SSLExportEntry.h"

// 0x823CBAB0 — pushes the SSLExportEntry describing haloENGINE_CONTROL::LoadCheckpoint. `id` is
// the byte offset of the paired `sslFuncId_LoadCheckpoint` slot (@ 0x18).
SSL_EXPORTER_haloENGINE_CONTROL::_SSL_FUNC_REGISTRATOR_LoadCheckpoint::_SSL_FUNC_REGISTRATOR_LoadCheckpoint()
{
    dsTSTRING<char> decl;
    decl.UnsafeInit("LoadCheckpoint(checkpoint: string)", -1, 0);

    SSL_EXPORTER_haloENGINE_CONTROL::SSLExportEntry entry(
        SSL_EXPORTER_haloENGINE_CONTROL::cbLoadCheckpoint,
        (int *)offsetof(SSL_EXPORTER_haloENGINE_CONTROL, sslFuncId_LoadCheckpoint),
        &decl);

    SSL_EXPORTER_haloENGINE_CONTROL::GetExportMap()->PushBack(entry);
}
