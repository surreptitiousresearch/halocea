/* ?RegisterEntry@SSL_EXPORTER_haloENGINE_CONTROL@@CAXP6AXVsslOBJ_REF@@HPAVdsDATA@@AAV3@0@ZPQ1@HABV?$dsTSTRING@D@@@Z @0x823CB8A0 */
#include "../../headers/hcex/ssl_exporter/SSLExportEntry.h"

// 0x823CB8AC — build one SSLExportEntry from (cb, id, sslDecl) and push it into GetExportMap().
void SSL_EXPORTER_haloENGINE_CONTROL::RegisterEntry(
    void (*cb)(sslOBJ_REF self, int argc, dsDATA *argv, dsDATA *retVal, sslOBJ_REF caller),
    int *id,
    const dsTSTRING<char> *sslDecl)
{
    SSLExportEntry entry(cb, id, sslDecl);
    dsVECTOR<SSLExportEntry, 8> *exportMap = SSL_EXPORTER_haloENGINE_CONTROL::GetExportMap();
    exportMap->PushBack(entry);
}
