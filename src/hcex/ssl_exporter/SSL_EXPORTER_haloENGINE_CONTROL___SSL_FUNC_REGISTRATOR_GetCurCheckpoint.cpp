#include <cstddef>
#include "../../headers/hcex/ssl_exporter/SSLExportEntry.h"

// @0x823CBEE0 — pushes the SSLExportEntry describing haloENGINE_CONTROL::GetCurCheckpoint. `id`
// is the byte offset of the paired `sslFuncId_GetCurCheckpoint` slot (@ 0x58).
SSL_EXPORTER_haloENGINE_CONTROL::_SSL_FUNC_REGISTRATOR_GetCurCheckpoint::_SSL_FUNC_REGISTRATOR_GetCurCheckpoint()
{
    dsTSTRING<char> decl;
    decl.UnsafeInit("GetCurCheckpoint(): string", -1, 0);

    SSL_EXPORTER_haloENGINE_CONTROL::SSLExportEntry entry(
        SSL_EXPORTER_haloENGINE_CONTROL::cbGetCurCheckpoint,
        (int *)offsetof(SSL_EXPORTER_haloENGINE_CONTROL, sslFuncId_GetCurCheckpoint),
        &decl);

    SSL_EXPORTER_haloENGINE_CONTROL::GetExportMap()->PushBack(entry);
}
