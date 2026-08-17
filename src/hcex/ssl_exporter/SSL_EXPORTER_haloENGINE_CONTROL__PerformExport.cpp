/* ?PerformExport@SSL_EXPORTER_haloENGINE_CONTROL@@QAAHVsslCLASS_REF@@@Z @0x823C9978 */
#include "../../headers/hcex/ssl_exporter/SSLExportEntry.h"
#include "../../headers/hcex/ssl_exporter/sslCLASS_REF_ssl_export_bridge.h"

// 0x823C9990
// DEVIATION: the DB's raw prototype/decompile for this method is a PPC struct-return-by-value ABI
// tangle: `sslClass` is mangled as a by-value `sslCLASS_REF` parameter (this ABI passes non-trivial
// by-value class args via a hidden pointer, which is what the decompiler shows as a plain
// `sslERROR *sslClass` parameter), and the two per-entry sret scratch locals (`v20`/`v22` in the
// raw decompile) are mistyped as `sslCLASS_REF` when they are really the `sslERROR` sret results
// of AddCbFunc/RegisterEvent (which share the same 12-byte stack-slot shape as a real
// sslCLASS_REF+padding). Retyped here to the DB-verified real kinds; the control flow and every
// field access are otherwise a direct translation of the decompile.
//
// Also: `empty_string` (the `errHint` argument at every AddCbFunc/RegisterEvent call site) is an
// un-named single zero byte at a fixed address — the compiler's coalesced empty-string constant,
// reconstructed here as a literal "".
int SSL_EXPORTER_haloENGINE_CONTROL::PerformExport(sslCLASS_REF *sslClass)
{
    int allOk = 1;
    dsVECTOR<SSLExportEntry, 8> *exportMap = SSL_EXPORTER_haloENGINE_CONTROL::GetExportMap();

    for (int i = 0; i < exportMap->Length(); ++i)
    {
        SSLExportEntry *entry = &(*exportMap)[i];
        sslERROR result;
        int resolvedId;

        if (entry->cb)
        {
            const char *declCStr = entry->sslDecl.CStr();
            sslCLASS_REF_bridge::AddCbFunc(sslClass, &result, declCStr, entry->cb, "", &resolvedId);
        }
        else
        {
            const char *declCStr = entry->sslDecl.CStr();
            sslCLASS_REF_bridge::RegisterEvent(sslClass, &result, declCStr, 0, "", &resolvedId);
        }

        // Stash the resolved id through the entry's `id` slot, interpreted as a byte offset into
        // *this (each _SSL_FUNC_REGISTRATOR_* ctor sets `id` to that offset — 0, 8, 16, ...).
        *(int *)((char *)this + (size_t)entry->id) = resolvedId;

        if (result.id)
        {
            _apLog("~Ssl,Error~SSL export failed: <%s> (%s: %d)",
                   entry->sslDecl.CStr(),
                   "d:\\projects\\code\\hcex\\sources\\halo_engine_ctrl.h",
                   76);
            allOk = 0;
            *(int *)((char *)this + (size_t)entry->id) = -1;
        }
    }

    return allOk;
}
