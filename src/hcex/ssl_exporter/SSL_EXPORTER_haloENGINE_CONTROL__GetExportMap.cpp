#include "../../headers/hcex/ssl_exporter/SSLExportEntry.h"

// 0x823C6E18 — Meyers-singleton local static: the shared, process-wide export table. Guarded by
// a compiler-emitted local-static-init guard; the table's dtor is registered with atexit() on
// first construction (see SSL_EXPORTER_haloENGINE_CONTROL__GetExportMap_atexit_dtor.cpp).
dsVECTOR<SSL_EXPORTER_haloENGINE_CONTROL::SSLExportEntry, 8> *SSL_EXPORTER_haloENGINE_CONTROL::GetExportMap()
{
    // DEVIATION: the decompiler shows the dsVECTOR ctor called with a bogus `(apCL*)0x4C` /
    // source-path-as-this argument pairing — a mis-attributed apCL{file,line} allocation cookie
    // (file = "d:\\projects\\code\\hcex\\sources\\halo_engine_ctrl.h", line = 0x4C = 76).
    // Reconstructed as the ordinary apCL-tagged ctor overload.
    static dsVECTOR<SSLExportEntry, 8> map(apCL{"d:\\projects\\code\\hcex\\sources\\halo_engine_ctrl.h", 76});
    return &map;
}
