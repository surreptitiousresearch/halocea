/* hcex_obj_sync_matrices_start @0x823D4E70 */
/* HCEX bridge boundary: hcexJOB_SYNC_INST_MNG is an opaque job-sync manager class. */
typedef struct hcexJOB_SYNC_INST_MNG hcexJOB_SYNC_INST_MNG;

/* Global instance-sync job manager (mangled: _hcexJobSyncMng__3UhcexJOB_SYNC_INST_MNG__A). */
extern hcexJOB_SYNC_INST_MNG hcexJobSyncMng;

/* Method (mangled: hcexJOB_SYNC_INST_MNG::StartSync(void)). */
void hcexJOB_SYNC_INST_MNG_StartSync(hcexJOB_SYNC_INST_MNG *self);

/* Kick off asynchronous synchronization of object instance matrices for the frame. */
extern "C" void hcex_obj_sync_matrices_start(void)
{
    hcexJOB_SYNC_INST_MNG_StartSync(&hcexJobSyncMng);
}
