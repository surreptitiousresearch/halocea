#include <cstddef>
#include "../../headers/hcex/ssl_exporter/SSLExportEntry.h"

// @0x823CBB98 — pushes the SSLExportEntry describing haloENGINE_CONTROL::RestartLevel. `id` is
// the byte offset of the paired `sslFuncId_RestartLevel` slot (@ 0x28).
SSL_EXPORTER_haloENGINE_CONTROL::_SSL_FUNC_REGISTRATOR_RestartLevel::_SSL_FUNC_REGISTRATOR_RestartLevel()
{
    dsTSTRING<char> decl;
    decl.UnsafeInit("RestartLevel()", -1, 0);

    SSL_EXPORTER_haloENGINE_CONTROL::SSLExportEntry entry(
        SSL_EXPORTER_haloENGINE_CONTROL::cbRestartLevel,
        (int *)offsetof(SSL_EXPORTER_haloENGINE_CONTROL, sslFuncId_RestartLevel),
        &decl);

    SSL_EXPORTER_haloENGINE_CONTROL::GetExportMap()->PushBack(entry);
}
