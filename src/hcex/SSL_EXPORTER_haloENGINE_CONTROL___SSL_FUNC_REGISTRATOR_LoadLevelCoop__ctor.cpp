#include "../headers/hcex/ssl_exporter/SSLExportEntry.h"
#include "../headers/ws/ds/dsVECTOR.h"
#include "../headers/ws/ds/dsTSTRING.h"

// SSL_EXPORTER_haloENGINE_CONTROL::_SSL_FUNC_REGISTRATOR_LoadLevelCoop::ctor @ 0x823CB978
// One-shot static-registration helper: push the LoadLevelCoop SSL export entry into the shared
// export table. id-slot byte offset is 8 (sslFuncId_LoadLevelCoop @ offset 8 of the exporter).
// DEVIATION: the decompiler inlines the SSLExportEntry field construction rather than calling its
// ctor; the (cb, id, sslDecl) ctor is the source-level equivalent.
SSL_EXPORTER_haloENGINE_CONTROL::_SSL_FUNC_REGISTRATOR_LoadLevelCoop::_SSL_FUNC_REGISTRATOR_LoadLevelCoop()
{
    dsTSTRING<char> decl(
        "LoadLevelCoop(map: string, difficulty: int, isNew: bool = false, isOnLine: bool = false)");
    SSL_EXPORTER_haloENGINE_CONTROL::SSLExportEntry entry(
        SSL_EXPORTER_haloENGINE_CONTROL::cbLoadLevelCoop,
        reinterpret_cast<int *>(8),
        &decl);
    SSL_EXPORTER_haloENGINE_CONTROL::GetExportMap()->PushBack(entry);
}
