#include "../../headers/hcex/ssl_exporter/SSLExportEntry.h"

// @0x84101DA0 — anonymous file-static function registered via atexit() the first time
// GetExportMap() runs; destroys the Meyers-singleton export table at process shutdown.
// (Compiler-synthesized name: `SSL_EXPORTER_haloENGINE_CONTROL::GetExportMap'::`2'::`dynamic
// atexit destructor for 'map''`.)
static void GetExportMap_dynamic_atexit_destructor_for_map()
{
    SSL_EXPORTER_haloENGINE_CONTROL::GetExportMap()->~dsVECTOR();
}
