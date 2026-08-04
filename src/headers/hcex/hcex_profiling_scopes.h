#pragma once
/* hcex_profiling_scopes — the apCOUNTER_TIME_CALL profiling scope enter/exit markers used
 * throughout the HCEX bridge per-frame code paths. In the binary these are the inline
 * apCOUNTER_TIME_CALL_WRP<apCOUNTER_TIME_CALL> RAII construct/destruct pairs the decompiler
 * expands into ~15 lines of per-thread timestamp bookkeeping each; the re-sourced corpus
 * collapses each nested scope into an enter()/exit() boundary pair (see
 * src/hcex/hcex_process_frame.c, hcex_create_effect_impl.c, hcex_obj_sync_matrices.c).
 *
 * enter() == apCOUNTER_TIME_CALL::Start (0x823DC758); exit() == apCOUNTER_TIME_CALL::Stop
 * (0x823DC928) on the counter named by the scope. */

#ifdef __cplusplus
extern "C" {
#endif

/* per-scope apCOUNTER_TIME_CALL profiling markers */
void cnt_hcex_main_loop_enter(void);
void cnt_hcex_main_loop_exit(void);
void cnt_hcex_obj_sync_enter(void);
void cnt_hcex_obj_sync_exit(void);
void cnt_hcex_fp_sync_enter(void);
void cnt_hcex_fp_sync_exit(void);
void cnt_hcex_create_sfx_enter(void);
void cnt_hcex_create_sfx_exit(void);
void cnt_hcex_process_frame_scope_enter(void);
void cnt_hcex_process_frame_scope_exit(void);

#ifdef __cplusplus
}
#endif
