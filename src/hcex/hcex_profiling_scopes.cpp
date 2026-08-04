/* hcex_profiling_scopes — definitions for the HCEX apCOUNTER_TIME_CALL profiling scope markers.
 *
 * Each of the five HCEX per-frame profiling scopes is backed by a global (or, for the outermost
 * process_frame scope, a function-local static) apCOUNTER_TIME_CALL. The enter/exit markers are
 * the inline apCOUNTER_TIME_CALL_WRP construct/destruct pair the compiler emitted at each scope;
 * they resolve to apCOUNTER_TIME_CALL::Start / ::Stop on the corresponding counter.
 *
 * The counters are constructed by their dynamic initializers (0x8409D2F0..0x8409D3F0 for the four
 * globals) and the outer scope's local static; see the DB. */

#include "../headers/hcex/hcex_profiling_scopes.h"
#include "../headers/ws/ap/apCOUNTER_TIME_CALL.h"

/* The five backing counters. The four global ones are constructed with a static-init name string
 * ("hcex/main loop", "hcex/obj sync", "hcex/fp sync", "hcex/create sfx"); the outermost
 * "hcex/process_frame" scope uses a function-local static in the original. */
extern apCOUNTER_TIME_CALL cnt_hcex_main_loop;   // 0x841C3AA8
extern apCOUNTER_TIME_CALL cnt_hcex_obj_sync;    // 0x841C3E54
extern apCOUNTER_TIME_CALL cnt_hcex_fp_sync;     // 0x841C3CC8
extern apCOUNTER_TIME_CALL cnt_hcex_create_sfx;  // 0x841C3BEC

/* apCOUNTER_TIME_CALL::Start (0x823DC758) / ::Stop (0x823DC928). */
extern "C++" {
    void apCOUNTER_TIME_CALL_Start(apCOUNTER_TIME_CALL *self); // ::Start
    void apCOUNTER_TIME_CALL_Stop(apCOUNTER_TIME_CALL *self);  // ::Stop
}

void cnt_hcex_main_loop_enter(void)  { apCOUNTER_TIME_CALL_Start(&cnt_hcex_main_loop); }
void cnt_hcex_main_loop_exit(void)   { apCOUNTER_TIME_CALL_Stop(&cnt_hcex_main_loop); }

void cnt_hcex_obj_sync_enter(void)   { apCOUNTER_TIME_CALL_Start(&cnt_hcex_obj_sync); }
void cnt_hcex_obj_sync_exit(void)    { apCOUNTER_TIME_CALL_Stop(&cnt_hcex_obj_sync); }

void cnt_hcex_fp_sync_enter(void)    { apCOUNTER_TIME_CALL_Start(&cnt_hcex_fp_sync); }
void cnt_hcex_fp_sync_exit(void)     { apCOUNTER_TIME_CALL_Stop(&cnt_hcex_fp_sync); }

void cnt_hcex_create_sfx_enter(void) { apCOUNTER_TIME_CALL_Start(&cnt_hcex_create_sfx); }
void cnt_hcex_create_sfx_exit(void)  { apCOUNTER_TIME_CALL_Stop(&cnt_hcex_create_sfx); }

/* Outermost per-frame scope: the original wraps a function-local static apCOUNTER_TIME_CALL named
 * "hcex/process_frame" constructed on first entry (with atexit teardown). */
void cnt_hcex_process_frame_scope_enter(void)
{
    static apCOUNTER_TIME_CALL cnt_hcex_process_frame("hcex/process_frame");
    apCOUNTER_TIME_CALL_Start(&cnt_hcex_process_frame);
}
void cnt_hcex_process_frame_scope_exit(void)
{
    static apCOUNTER_TIME_CALL cnt_hcex_process_frame("hcex/process_frame");
    apCOUNTER_TIME_CALL_Stop(&cnt_hcex_process_frame);
}
