/* ?cbLoadLevelCoop@SSL_EXPORTER_haloENGINE_CONTROL@@SAXVsslOBJ_REF@@HPAVdsDATA@@AAV3@0@Z @0x823CB088 */
#include "../../headers/hcex/ssl_exporter/SSL_EXPORTER_haloENGINE_CONTROL.h"
#include "../../headers/hcex/ssl_exporter/sslOBJ_REF_ssl_export_bridge.h"

// 0x823CB0BC
// DEVIATION: see cbRestartLevel.cpp — `v11.HandleCall(method, host, ...)` collapsed to
// `method->HandleCall(host, ...)`.
void SSL_EXPORTER_haloENGINE_CONTROL::cbLoadLevelCoop(sslOBJ_REF self, int argc, dsDATA *argv, dsDATA *retVal, sslOBJ_REF caller)
{
    static ds_data_call::HandlerBase_haloENGINE_CONTROL *method =
        ds_data_call::CreateHandler<haloENGINE_CONTROL, haloENGINE_CONTROL, void, dsTSTRING<char>, short, bool, bool>(&haloENGINE_CONTROL::LoadLevelCoop);

    sslCB_HOST *cbHost = sslOBJ_REF_bridge::GetCbHost(&self);
    method->HandleCall((haloENGINE_CONTROL *)cbHost, argv, argc, retVal);
}
