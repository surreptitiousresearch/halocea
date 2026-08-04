#include "../../headers/hcex/ssl_exporter/SSL_EXPORTER_haloENGINE_CONTROL.h"
#include "../../headers/hcex/ssl_exporter/sslOBJ_REF_ssl_export_bridge.h"

// 0x823C4C4C
// DEVIATION: the decompiler reconstructs this virtual dispatch as a copy of `method`'s vtable
// pointer into a bogus stack local, then calls through it while re-passing `method` itself as a
// leading argument (`v11.HandleCall(method, host, argv, argc, retVal)`). That is PPC virtual-call
// codegen (the vtable pointer is loaded once into a register and reused both to fetch the
// HandleCall slot and as the call's real receiver) — not a value the source ever named.
// Reconstructed as the plain virtual call `method->HandleCall(host, argv, argc, retVal)`.
void SSL_EXPORTER_haloENGINE_CONTROL::cbRestartLevel(sslOBJ_REF self, int argc, dsDATA *argv, dsDATA *retVal, sslOBJ_REF caller)
{
    static ds_data_call::HandlerBase_haloENGINE_CONTROL *method =
        ds_data_call::CreateHandler<haloENGINE_CONTROL, haloENGINE_CONTROL, void>(&haloENGINE_CONTROL::RestartLevel);

    sslCB_HOST *cbHost = sslOBJ_REF_bridge::GetCbHost(&self);
    method->HandleCall((haloENGINE_CONTROL *)cbHost, argv, argc, retVal);
}
