#include <cstddef>
#include "../../headers/hcex/ssl_exporter/SSLExportEntry.h"

// @0x823CBF68 — pushes the SSLExportEntry describing haloENGINE_CONTROL::GetCurLevel. `id` is the
// byte offset of the paired `sslFuncId_GetCurLevel` slot (@ 0x60).
SSL_EXPORTER_haloENGINE_CONTROL::_SSL_FUNC_REGISTRATOR_GetCurLevel::_SSL_FUNC_REGISTRATOR_GetCurLevel()
{
    dsTSTRING<char> decl;
    decl.UnsafeInit("GetCurLevel(): string", -1, 0);

    SSL_EXPORTER_haloENGINE_CONTROL::SSLExportEntry entry(
        SSL_EXPORTER_haloENGINE_CONTROL::cbGetCurLevel,
        (int *)offsetof(SSL_EXPORTER_haloENGINE_CONTROL, sslFuncId_GetCurLevel),
        &decl);

    SSL_EXPORTER_haloENGINE_CONTROL::GetExportMap()->PushBack(entry);
}
