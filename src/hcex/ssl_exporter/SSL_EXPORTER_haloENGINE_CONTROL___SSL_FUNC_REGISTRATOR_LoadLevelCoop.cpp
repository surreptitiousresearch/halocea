#include <cstddef>
#include "../../headers/hcex/ssl_exporter/SSLExportEntry.h"

// 0x823CB9A0 — pushes the SSLExportEntry describing haloENGINE_CONTROL::LoadLevelCoop. `id` is
// the byte offset of the paired `sslFuncId_LoadLevelCoop` slot (@ 0x08).
SSL_EXPORTER_haloENGINE_CONTROL::_SSL_FUNC_REGISTRATOR_LoadLevelCoop::_SSL_FUNC_REGISTRATOR_LoadLevelCoop()
{
    dsTSTRING<char> decl;
    decl.UnsafeInit("LoadLevelCoop(map: string, difficulty: int, isNew: bool = false, isOnLine: bool = false)", -1, 0);

    SSL_EXPORTER_haloENGINE_CONTROL::SSLExportEntry entry(
        SSL_EXPORTER_haloENGINE_CONTROL::cbLoadLevelCoop,
        (int *)offsetof(SSL_EXPORTER_haloENGINE_CONTROL, sslFuncId_LoadLevelCoop),
        &decl);

    SSL_EXPORTER_haloENGINE_CONTROL::GetExportMap()->PushBack(entry);
}
