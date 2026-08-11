#include <cstddef>
#include "../../headers/hcex/ssl_exporter/SSLExportEntry.h"

// @0x823CBE58 — pushes the SSLExportEntry describing haloENGINE_CONTROL::IsCoop. `id` is the byte
// offset of the paired `sslFuncId_IsCoop` slot (@ 0x50).
SSL_EXPORTER_haloENGINE_CONTROL::_SSL_FUNC_REGISTRATOR_IsCoop::_SSL_FUNC_REGISTRATOR_IsCoop()
{
    dsTSTRING<char> decl;
    decl.UnsafeInit("IsCoop(): bool", -1, 0);

    SSL_EXPORTER_haloENGINE_CONTROL::SSLExportEntry entry(
        SSL_EXPORTER_haloENGINE_CONTROL::cbIsCoop,
        (int *)offsetof(SSL_EXPORTER_haloENGINE_CONTROL, sslFuncId_IsCoop),
        &decl);

    SSL_EXPORTER_haloENGINE_CONTROL::GetExportMap()->PushBack(entry);
}
