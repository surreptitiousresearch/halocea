/* hcex_obj_sync_matrices @0x823DD8C0 — profiled driver that finishes the per-frame object-matrix-sync
 * pass: runs hcex_obj_sync_game_state() over every tracked HCEX_OBJ inside a "hcex_obj_sync" PIX event,
 * then blocks on hcexJOB_SYNC_INST_MNG::StopSync to join the async instance-sync job that
 * hcex_obj_sync_matrices_start kicked off earlier in the frame. The whole body runs inside the
 * cnt_hcex_obj_sync apCOUNTER_TIME_CALL profiling scope.
 *
 * deviation: the decompiler flags this function with "local variable allocation has failed" and renders
 * it as an 8-argument function operating on register-garbage stack slots — an artifact of the inlined
 * apCOUNTER macro's heavy stack usage defeating IDA's variable allocator. Disasm confirms the real
 * signature is `void hcex_obj_sync_matrices(void)` (mangled `?hcex_obj_sync_matrices@@YAXXZ`, called with
 * no arguments from hcex_postproc_frame/hcex_process_frame); the two real calls in the body
 * (hcex_obj_sync_game_state, hcexJOB_SYNC_INST_MNG::StopSync) are reconstructed directly and the
 * apCOUNTER scaffolding is collapsed into the enter/exit boundary pair per this corpus's convention. */

#include "../headers/hcex/hcex_obj_sync_matrices_boundary.h"

void hcex_obj_sync_matrices(void)
{
    cnt_hcex_obj_sync_enter();

    osPIXBeginEvent("hcex_obj_sync");
    hcex_obj_sync_game_state();
    hcexJobSyncMng.StopSync();
    osPIXEndEvent();

    cnt_hcex_obj_sync_exit();
}
