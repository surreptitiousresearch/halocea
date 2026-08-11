#include <cstddef>
#include "../../headers/hcex/ssl_exporter/SSLExportEntry.h"

// @0x823CBDD0 — pushes the SSLExportEntry describing haloENGINE_CONTROL::SetMode. `id` is the
// byte offset of the paired `sslFuncId_SetMode` slot (@ 0x48).
SSL_EXPORTER_haloENGINE_CONTROL::_SSL_FUNC_REGISTRATOR_SetMode::_SSL_FUNC_REGISTRATOR_SetMode()
{
    dsTSTRING<char> decl;
    decl.UnsafeInit("SetMode(isCoop: bool)", -1, 0);

    SSL_EXPORTER_haloENGINE_CONTROL::SSLExportEntry entry(
        SSL_EXPORTER_haloENGINE_CONTROL::cbSetMode,
        (int *)offsetof(SSL_EXPORTER_haloENGINE_CONTROL, sslFuncId_SetMode),
        &decl);

    SSL_EXPORTER_haloENGINE_CONTROL::GetExportMap()->PushBack(entry);
}
