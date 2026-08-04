#include <cstddef>
#include "../../headers/hcex/ssl_exporter/SSLExportEntry.h"

// 0x823CB918 — pushes the SSLExportEntry describing haloENGINE_CONTROL::LoadLevelSolo. `id` is
// reconstructed as the byte offset of this exporter's paired `sslFuncId_LoadLevelSolo` slot
// (DB-verified layout: SSL_EXPORTER_haloENGINE_CONTROL.sslFuncId_LoadLevelSolo @ 0x00), matching
// the decompile's raw `(int*)0` literal.
SSL_EXPORTER_haloENGINE_CONTROL::_SSL_FUNC_REGISTRATOR_LoadLevelSolo::_SSL_FUNC_REGISTRATOR_LoadLevelSolo()
{
    dsTSTRING<char> decl;
    decl.UnsafeInit("LoadLevelSolo(map: string, difficulty: int, isNew: bool = false)", -1, 0);

    SSL_EXPORTER_haloENGINE_CONTROL::SSLExportEntry entry(
        SSL_EXPORTER_haloENGINE_CONTROL::cbLoadLevelSolo,
        (int *)offsetof(SSL_EXPORTER_haloENGINE_CONTROL, sslFuncId_LoadLevelSolo),
        &decl);

    SSL_EXPORTER_haloENGINE_CONTROL::GetExportMap()->PushBack(entry);
}
