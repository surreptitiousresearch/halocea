#pragma once
/* Boundary declarations for hcex_preproc_frame.c.
 *
 * hcex_preproc_frame runs once at the top of the HCEX per-frame update: it destroys any scene instances
 * queued for deferred removal, updates the render/kinect modes, handles the debug
 * "force local-player index" coop toggle, and reconciles the ws-engine ENGINE_CONTROL's split-screen
 * flag with hcex_split_screen while outside a cinematic in legacy (Blam) render mode. ENGINE_CONTROL is
 * the C++ control object wrapping Blam engine state for the outer engine (see CLAUDE.md); only the two
 * methods touched here are modeled. */

#include <stdint.h>
#include "hcex_dbgvar_boundary.h"   /* dbgVAR_bool */

typedef struct ENGINE_CONTROL ENGINE_CONTROL;
extern ENGINE_CONTROL *gsEngineCtrl;
extern int  ENGINE_CONTROL_IsSplitScreen(const ENGINE_CONTROL *self);      /* IsSplitScreen() const */
extern void ENGINE_CONTROL_SetSplitScreen(ENGINE_CONTROL *self, int value); /* SetSplitScreen(bool) */

extern "C" int  hcex_coop_local_player_index;   /* int */
extern int  hcex_cur_render_mode;
extern bool hcex_split_screen;
extern bool hcex_enable_pix;

extern "C" dbgVAR_bool dbg_hcex_test_online_coop_switch_plr;

extern "C" void hcex_pix_begin_event(const char *fmt, ...);   /* already reversed: hcex_pix_begin_event.c */
extern void osPIXEndEvent(void);

extern void hcex_destroy_dead_intances(void);   /* already reversed: hcex_destroy_dead_intances.c */
extern void hcex_change_render_mode(void);      /* not in this batch — boundary */
extern void hcex_change_kinect_state(void);     /* already reversed: hcex_change_kinect_state.c */
extern "C" uint8_t cinematic_in_progress(void);    /* Blam engine — distinct from cinematic_globals->cinematic_in_progress */
