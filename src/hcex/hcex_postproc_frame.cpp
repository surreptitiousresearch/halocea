/* hcex_postproc_frame @0x823DF180 — closes out the HCEX per-frame update: ends the PIX event opened by
 * hcex_preproc_frame (when PIX instrumentation is enabled), then — if an object-sync-matrices pass was
 * left in progress by the job system — runs it synchronously here.
 *
 * deviation: the decompiler renders this as a 7-argument passthrough to hcex_obj_sync_matrices (a1..a7),
 * an artifact of hcex_obj_sync_matrices' own stack-slot-allocation failure (see its file); disasm
 * confirms both hcex_postproc_frame and the hcex_obj_sync_matrices call site take/pass no real
 * arguments (`bl hcex_obj_sync_matrices(void)` with no register setup beyond the conditional branch). */

#include "../headers/hcex/hcexCallFrame_boundary.h"

extern void osPIXEndEvent(void);

void hcex_postproc_frame(void)
{
    if ( hcex_enable_pix )
        osPIXEndEvent();

    if ( hcex_obj_sync_matrices_in_progress )
        hcex_obj_sync_matrices();
}
