#pragma once
/* Boundary declarations for hcex_obj_sync_matrices.c.
 *
 * hcex_obj_sync_matrices is the profiled driver that finishes the per-frame object-matrix-sync job: it
 * runs hcex_obj_sync_game_state() (over every tracked HCEX_OBJ) inside a "hcex_obj_sync" PIX event, then
 * blocks on hcexJOB_SYNC_INST_MNG::StopSync to join whatever async instance-sync work
 * hcex_obj_sync_matrices_start kicked off earlier in the frame (see hcex_obj_sync_matrices_start.c).
 * The whole body is bracketed by the cnt_hcex_obj_sync apCOUNTER_TIME_CALL profiling scope. */

/* hcexJOB_SYNC_INST_MNG — full C++ type already reversed at ../hcexJOB_SYNC_INST_MNG.h; reuse it
 * (rather than the opaque flat declaration hcex_obj_sync_matrices_start.c uses) so StopSync is
 * type-checkable as a real method call. */
#include "hcexJOB_SYNC_INST_MNG.h"

extern void hcex_obj_sync_game_state(void);   /* one of this batch's targets */

extern void osPIXBeginEvent(const char *string);
extern void osPIXEndEvent(void);

/* apCOUNTER_TIME_CALL profiling scope for "cnt_hcex_obj_sync" (see apCOUNTER_TIME_CALL.h). The original
 * inlines the apCOUNTER macro (per-thread start-timestamp capture, callsTotal increment, myProfiler
 * Start/Stop) around the body; reproduced here as enter/exit boundary calls per this corpus's
 * established convention (see hcex_create_effect_impl.c's cnt_hcex_create_sfx_enter/exit). */
extern void cnt_hcex_obj_sync_enter(void);
extern void cnt_hcex_obj_sync_exit(void);
