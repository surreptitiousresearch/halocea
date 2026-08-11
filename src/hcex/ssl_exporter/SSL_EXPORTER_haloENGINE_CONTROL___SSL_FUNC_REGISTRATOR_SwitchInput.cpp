#include <cstddef>
#include "../../headers/hcex/ssl_exporter/SSLExportEntry.h"

// @0x823CC958 — pushes the SSLExportEntry describing haloENGINE_CONTROL::SwitchInput. `id` is
// the byte offset of the paired `sslFuncId_SwitchInput` slot (@ 0x38).
SSL_EXPORTER_haloENGINE_CONTROL::_SSL_FUNC_REGISTRATOR_SwitchInput::_SSL_FUNC_REGISTRATOR_SwitchInput()
{
    dsTSTRING<char> decl;
    decl.UnsafeInit("SwitchInput(isOn: bool)", -1, 0);

    SSL_EXPORTER_haloENGINE_CONTROL::SSLExportEntry entry(
        SSL_EXPORTER_haloENGINE_CONTROL::cbSwitchInput,
        (int *)offsetof(SSL_EXPORTER_haloENGINE_CONTROL, sslFuncId_SwitchInput),
        &decl);

    SSL_EXPORTER_haloENGINE_CONTROL::GetExportMap()->PushBack(entry);
}
