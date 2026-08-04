#include <cstddef>
#include "../../headers/hcex/ssl_exporter/SSLExportEntry.h"

// 0x823CBA28 — pushes the SSLExportEntry describing haloENGINE_CONTROL::LoadMainMenu. `id` is
// the byte offset of the paired `sslFuncId_LoadMainMenu` slot (@ 0x10).
SSL_EXPORTER_haloENGINE_CONTROL::_SSL_FUNC_REGISTRATOR_LoadMainMenu::_SSL_FUNC_REGISTRATOR_LoadMainMenu()
{
    dsTSTRING<char> decl;
    decl.UnsafeInit("LoadMainMenu()", -1, 0);

    SSL_EXPORTER_haloENGINE_CONTROL::SSLExportEntry entry(
        SSL_EXPORTER_haloENGINE_CONTROL::cbLoadMainMenu,
        (int *)offsetof(SSL_EXPORTER_haloENGINE_CONTROL, sslFuncId_LoadMainMenu),
        &decl);

    SSL_EXPORTER_haloENGINE_CONTROL::GetExportMap()->PushBack(entry);
}
