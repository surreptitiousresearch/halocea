#include <cstddef>
#include "../../headers/hcex/ssl_exporter/SSLExportEntry.h"

// 0x823CBC48 — pushes the SSLExportEntry describing haloENGINE_CONTROL::CreateCheckpointList.
// `id` is the byte offset of the paired `sslFuncId_CreateCheckpointList` slot (@ 0x30).
SSL_EXPORTER_haloENGINE_CONTROL::_SSL_FUNC_REGISTRATOR_CreateCheckpointList::_SSL_FUNC_REGISTRATOR_CreateCheckpointList()
{
    dsTSTRING<char> decl;
    decl.UnsafeInit("CreateCheckpointList(level: string): vector", -1, 0);

    SSL_EXPORTER_haloENGINE_CONTROL::SSLExportEntry entry(
        SSL_EXPORTER_haloENGINE_CONTROL::cbCreateCheckpointList,
        (int *)offsetof(SSL_EXPORTER_haloENGINE_CONTROL, sslFuncId_CreateCheckpointList),
        &decl);

    SSL_EXPORTER_haloENGINE_CONTROL::GetExportMap()->PushBack(entry);
}
