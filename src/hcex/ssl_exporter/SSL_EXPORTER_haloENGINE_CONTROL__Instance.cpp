#include "../../headers/hcex/ssl_exporter/SSLExportEntry.h"

// 0x823CCA98 — Meyers-singleton local static: the one process-wide SSL_EXPORTER_haloENGINE_CONTROL,
// whose ctor runs all 13 registrators on first access.
SSL_EXPORTER_haloENGINE_CONTROL *SSL_EXPORTER_haloENGINE_CONTROL::Instance()
{
    static SSL_EXPORTER_haloENGINE_CONTROL inst;
    return &inst;
}
