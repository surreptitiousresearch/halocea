#include "../../headers/hcex/ssl_exporter/SSLExportEntry.h"

// 0x823CC9F8 — private default ctor: member-initializes all 13 (id-slot, registrator) pairs in
// declaration order. The `sslFuncId_*` int slots are left uninitialized here (as in the decompile
// — PerformExport() is what first writes them); each registrator ctor's side effect is pushing
// its SSLExportEntry into the shared GetExportMap() table.
SSL_EXPORTER_haloENGINE_CONTROL::SSL_EXPORTER_haloENGINE_CONTROL()
    : _ssl_func_registrator_LoadLevelSolo()
    , _ssl_func_registrator_LoadLevelCoop()
    , _ssl_func_registrator_LoadMainMenu()
    , _ssl_func_registrator_LoadCheckpoint()
    , _ssl_func_registrator_RestartCheckpoint()
    , _ssl_func_registrator_RestartLevel()
    , _ssl_func_registrator_CreateCheckpointList()
    , _ssl_func_registrator_SwitchInput()
    , _ssl_func_registrator_SetPause()
    , _ssl_func_registrator_SetMode()
    , _ssl_func_registrator_IsCoop()
    , _ssl_func_registrator_GetCurCheckpoint()
    , _ssl_func_registrator_GetCurLevel()
{
}
