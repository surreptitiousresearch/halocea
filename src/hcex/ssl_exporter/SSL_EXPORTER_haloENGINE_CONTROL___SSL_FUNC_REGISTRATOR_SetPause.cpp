#include <cstddef>
#include "../../headers/hcex/ssl_exporter/SSLExportEntry.h"

// 0x823CBD70 — pushes the SSLExportEntry describing haloENGINE_CONTROL::SetPause. `id` is the
// byte offset of the paired `sslFuncId_SetPause` slot (@ 0x40).
SSL_EXPORTER_haloENGINE_CONTROL::_SSL_FUNC_REGISTRATOR_SetPause::_SSL_FUNC_REGISTRATOR_SetPause()
{
    dsTSTRING<char> decl;
    decl.UnsafeInit("SetPause(isOn: bool)", -1, 0);

    SSL_EXPORTER_haloENGINE_CONTROL::SSLExportEntry entry(
        SSL_EXPORTER_haloENGINE_CONTROL::cbSetPause,
        (int *)offsetof(SSL_EXPORTER_haloENGINE_CONTROL, sslFuncId_SetPause),
        &decl);

    SSL_EXPORTER_haloENGINE_CONTROL::GetExportMap()->PushBack(entry);
}
