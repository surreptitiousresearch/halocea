#include "../headers/hcex/ssl_exporter/SSLExportEntry.h"
#include "../headers/ws/ds/dsVECTOR.h"
#include "../headers/ws/ds/dsTSTRING.h"

// SSL_EXPORTER_haloENGINE_CONTROL::_SSL_FUNC_REGISTRATOR_LoadLevelSolo::ctor @ 0x823CB8F0
// One-shot static-registration helper: push the LoadLevelSolo SSL export entry into the shared
// export table. id-slot byte offset is 0 (sslFuncId_LoadLevelSolo @ offset 0 of the exporter).
// DEVIATION: the decompiler inlines the SSLExportEntry field construction rather than calling its
// ctor; the (cb, id, sslDecl) ctor is the source-level equivalent.
SSL_EXPORTER_haloENGINE_CONTROL::_SSL_FUNC_REGISTRATOR_LoadLevelSolo::_SSL_FUNC_REGISTRATOR_LoadLevelSolo()
{
    dsTSTRING<char> decl("LoadLevelSolo(map: string, difficulty: int, isNew: bool = false)");
    SSL_EXPORTER_haloENGINE_CONTROL::SSLExportEntry entry(
        SSL_EXPORTER_haloENGINE_CONTROL::cbLoadLevelSolo,
        reinterpret_cast<int *>(0),
        &decl);
    SSL_EXPORTER_haloENGINE_CONTROL::GetExportMap()->PushBack(entry);
}
