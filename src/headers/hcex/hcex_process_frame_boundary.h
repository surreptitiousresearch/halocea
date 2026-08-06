#pragma once
/* Boundary declarations for hcex_process_frame.c.
 *
 * hcex_process_frame is the top-level per-frame entry point for the HCEX bridge (called once per
 * real frame, gated on hcex_call_frame && main_coop_update()): it snapshots the cheat-toggle globals
 * from their debug-console vars, runs the legacy Blam main loop update (main_loop_update) under the
 * appropriate render-mode lock, syncs first-person weapon state (hcex_fp_sync), dispatches the
 * legacy-render job (or marks the frame render-ready) depending on render mode, then runs a long tail
 * of debug-console-driven one-shot actions (delayed decal/effect/light creation, checkpoint-save,
 * cheat teleport/camo, legacy-mode toggle, god-mode toggle, stereo-3D toggle, material-effect-type
 * re-resolve, "always shoot" debounce, and a dozen dump/clear-list debug commands) before closing out
 * its two PIX events.
 *
 * NOTE ON SCOPE: this function reads roughly forty distinct HCEX debug-console variables
 * (dbg_hcex_* / dbg_cheat*). Each is declared below with its value type inferred from how
 * hcex_process_frame uses it (bool toggle vs. float knob vs. string), matching the dbgVAR_bool /
 * dbgVAR_float / dbgVAR_STRING shapes already DB-verified in hcex_dbgvar_boundary.h — but, unlike the
 * rest of this batch, their *exact* DB member layout was not individually re-queried one-by-one given
 * this function's size; treat the type (bool/float/string) as verified-by-usage, and the byte offsets
 * as inherited from the already-DB-verified dbgVAR_bool/dbgVAR_float/dbgVAR_STRING shapes rather than
 * independently re-checked per variable.
 *
 * The apCOUNTER_TIME_CALL profiling scaffolding (both the whole-function RAII scope timer backed by
 * the function-local static `localTimeCounter`/global `cnt_hcex_main_loop`, and the narrower
 * "hcex_main_loop"/"hcex_fp_sync" sub-scopes) is collapsed into enter/exit boundary call pairs per
 * this corpus's established convention (see hcex_create_effect_impl.c's cnt_hcex_create_sfx_enter/exit
 * and hcex_obj_sync_matrices.c's cnt_hcex_obj_sync_enter/exit). */

#include <stdint.h>
#include "hcex_dbgvar_boundary.h"     /* dbgVAR_bool, dbgVAR_float, dbgVAR_STRING, dbgVAR_STRING_CStr */
#include "hcex_ds_boundary.h"         /* dsTSTRING_flat, dsSTRID, dsEVENT_MGR, gEventMgr, dlFree */
#include "hcex_param_list_boundary.h" /* dsVECTOR_PARAM_LIST, dsPARAM_LIST helpers */
#include "../cheat_globals.h"         /* cheat_globals, cheat */
#include "../ws/os/osLOCK.h"          /* osLOCK (hcexHaloLogic) */

/* --- cheat-toggle source debug vars (dbgVAR_SIMPLE<bool,1>) --- */
extern "C" dbgVAR_bool dbg_cheatDeathless;
extern "C" dbgVAR_bool dbg_cheatInfAmmo;
extern "C" dbgVAR_bool dbg_cheatBumpPossession;
extern "C" dbgVAR_bool dbg_cheatReflexiveDamageEffects;
extern "C" dbgVAR_bool dbg_cheatMedusa;
extern "C" dbgVAR_bool dbg_cheatSuperJump;
extern "C" dbgVAR_bool dbg_cheatOmnipotent;
extern "C" dbgVAR_bool dbg_cheatBottomlessClip;
extern bool         hcex_infinite_ammo;

/* --- per-frame drop/spawn cheat toggles --- */
extern "C" dbgVAR_bool   dbg_hcex_DropWeapons;
extern "C" dbgVAR_bool   dbg_hcex_DropVehicles;
extern "C" dbgVAR_bool   dbg_hcex_DropCharacters;
extern "C" dbgVAR_bool   dbg_hcex_DropPowerups;
extern "C" dbgVAR_bool   dbg_hcex_DropByName;
extern "C" dbgVAR_STRING dbg_hcex_ObjName;
extern "C" dbgVAR_bool   dbg_hcex_DropVarByName;
extern "C" dbgVAR_STRING dbg_hcex_VarName;
extern "C" dbgVAR_bool   dbg_hcex_ReloadShaders;

/* --- checkpoint-save toggles --- */
extern "C" dbgVAR_bool dbg_hcex_GenChpt;
extern "C" dbgVAR_bool dbg_hcex_GenSafe;

/* --- misc cheats/dumps --- */
extern "C" dbgVAR_bool   dbg_hcex_TeleportToCamera;
extern "C" dbgVAR_bool   dbg_hcex_ActiveCamo;
extern "C" dbgVAR_bool   dbg_hcex_DumpVariants;
extern "C" dbgVAR_bool   dbg_hcex_SwitchMode;
extern "C" dbgVAR_float  dbg_hcex_always_shoot_period;
extern "C" dbgVAR_bool   dbg_hcex_always_shoot;
extern "C" dbgVAR_bool   dbg_hcex_DrawDebug;
extern "C" dbgVAR_bool   dbg_hcex_EffectMoveTo;
extern "C" dbgVAR_STRING dbg_hcex_Effect;
extern "C" dbgVAR_float  dbg_hcex_flySpeedAccel;
extern "C" dbgVAR_bool   dbg_hcexDumpUsedSfx;
extern "C" dbgVAR_bool   dbg_hcexClearUsedSfx;
extern "C" dbgVAR_bool   dbg_hcexDumpUsedDecals;
extern "C" dbgVAR_bool   dbg_hcexClearUsedDecals;
extern "C" dbgVAR_bool   dbg_hcexDumpMissingSfx;
extern "C" dbgVAR_bool   dbg_hcexDumpUsedCls;
extern "C" dbgVAR_bool   dbg_hcexClearUsedCls;
extern "C" dbgVAR_bool   dbg_hcexDumpUsedTex;
extern "C" dbgVAR_bool   dbg_hcexClearUsedTex;
extern "C" dbgVAR_bool   dbg_hcexDumpScenery;
extern "C" dbgVAR_bool   dbg_hcex_SwitchCam;
extern "C" dbgVAR_bool   dbg_hcex_DrawIgnoreZ;
extern "C" dbgVAR_bool   dbg_hcex_DrawEffects;
extern "C" dbgVAR_STRING dbg_hcex_PjlEffect;
extern "C" dbgVAR_STRING dbg_hcex_MtlType;
extern "C" dbgVAR_bool   dbg_hcex_AiIsActive;

/* --- runtime globals this function reads/writes --- */
extern bool  hcex_call_frame;
extern int   hcex_cur_render_mode;
extern "C" int   hcex_switch_cam_mode;
extern "C" int   hcex_dbg_ignore_z;
extern "C" int   hcex_debug_effect_names;
extern "C" int   hcex_always_shoot;
extern "C" int   hcex_material_effect_type;
extern "C" float hcex_fly_mode_speed_scale;
extern bool  hcex_render_ready;
extern "C" uint8_t   debug_objects;
extern const char *const hcex_mtl_effects[0x21];   /* 33-entry material-effect-type name table */

#include "../ws/ds/dsVECTOR.h"
typedef dsVECTOR<dsTSTRING<char>, 8> dsTSTRING_vector; /* DB dsVECTOR<dsTSTRING<char>,8> */
extern dsTSTRING_vector hcexUsedSfx;
extern dsTSTRING_vector hcexUsedDecals;
extern dsTSTRING_vector hcexMissedSfx;
extern dsTSTRING_vector hcexClsList;
extern dsTSTRING_vector hcexTexList;
extern void dsTSTRING_vector_Clear(dsTSTRING_vector *v);

/* --- apCONFIG "LegacyMode" persisted setting --- */
typedef struct apCONFIG apCONFIG;
extern apCONFIG *gCfg;
extern int  apCONFIG_RetrieveInt(apCONFIG *cfg, const char *section, const char *key, int *out, const int *fallback);
extern void apCfgWriteInt(const char *section, const char *key, int value);

/* --- gsINP_SYSTEM debug binding context ("dbg_toggle_halo_window" / "dbg_god_mode" /
 * "dbg_toggle_stereo_3D" IsJustOn checks) --- */
typedef struct gsINP_SYSTEM gsINP_SYSTEM;
typedef struct gsiCONTEXT gsiCONTEXT;
extern gsINP_SYSTEM *gsSysInput;
extern gsiCONTEXT    *gsINP_SYSTEM_GetDbgContext(gsINP_SYSTEM *self);
extern int            gsiCONTEXT_IsJustOn(gsiCONTEXT *self, const dsSTRID *action);

/* --- render system / stereo-3D ---
 * deviation: the decompiled `vidDriver->curConfig.stereo3D` and `gsSysRender->stereo3DRatio` field
 * writes aren't backed by a DB-verified struct offset in this batch (vidDRIVER/gsRENDER_SYSTEM are
 * large self-contained ws-engine classes only partially modeled elsewhere in this corpus, and neither
 * of those two specific fields was re-queried here); reproduced as accessor-function boundaries
 * instead of guessed raw offsets, per the project's "don't invent a layout" convention. */
typedef struct gsRENDER_SYSTEM gsRENDER_SYSTEM;
typedef struct vidDRIVER vidDRIVER;
extern gsRENDER_SYSTEM *gsSysRender;
extern vidDRIVER       *vidDriver;
extern void gsRENDER_SYSTEM_ToggleStereo3D(gsRENDER_SYSTEM *self);
extern int  gsRENDER_SYSTEM_GetIsStereo3D(const gsRENDER_SYSTEM *self);       /* isStereo3D */
extern void gsRENDER_SYSTEM_SetStereo3DRatio(gsRENDER_SYSTEM *self, float ratio); /* stereo3DRatio = value */
extern void vidDRIVER_SetCurConfigStereo3D(vidDRIVER *self, int stereo3D);    /* curConfig.stereo3D = value */
extern "C" uint8_t director_is_first_person(int16_t local_player_index);

/* --- god-mode toggle helper (already reversed: hcex_god_mode_toggle.c) --- */
extern "C" void hcex_god_mode_toggle(void);

/* --- dbgVAR_MANAGER (already fully typed: ws/dbg/dbgVAR_MANAGER.h); the string-name lookup path
 * used here (SetDbgVar<bool,1> by dotted display name) is declared directly. */
typedef struct dbgVAR_MANAGER dbgVAR_MANAGER;
extern dbgVAR_MANAGER *dbgVAR_MANAGER_GetManager(void);
extern void dbgVAR_MANAGER_SetDbgVar_bool(dbgVAR_MANAGER *mgr, const dsTSTRING_flat *name, const bool *value);

/* --- the rest of the per-frame pipeline (already reversed elsewhere in this corpus) --- */
extern int  main_coop_update(void);
extern void gsRendPrepareRenderNextFrameSkip(void);
extern "C" int main_loop_update(void);
extern void hcex_draw_string_flush(void);
extern "C" void hcex_fp_sync(void);
extern "C" void hcex_shaders_fx_update(void);
extern "C" void main_loop_check_state(void);
extern "C" int16_t local_player_count(void);
extern void hcex_light_sync_all(void);
extern void hcex_create_effects_delayed(void);
extern void hcex_create_decals_delayed(void);
extern "C" void main_save_map_safe(void);
extern "C" void main_save_map_nonsafe(void);
extern "C" void cheat_teleport_to_camera(void);
extern "C" int cheat_active_camouflage(void);
extern "C" void hcex_dump_variants(void);
extern void hcex_update_skulls(void);
extern "C" void hcex_set_pjl_effect(const char *name);
extern "C" void ai_globals_ai_active(uint8_t enable);
extern "C" void effect_move_to_next(const char *eff_name);
extern "C" void hcex_dump_scenery(void);
extern "C" void hcex_change_screen_mode(int mode);

/* --- debug "drop everything" cheats + spawn-by-name (not this batch's targets) --- */
extern "C" void cheat_all_weapons(void);
extern "C" void cheat_all_vehicles(void);
extern "C" void cheat_all_chars(void);
extern "C" void cheat_all_powerups(void);
extern "C" void cheat_by_model(const char *mdl_name);
extern "C" void spawn_variant_near_player(const char *variant_name);

extern void dsTSTRING_assign(dsTSTRING_flat *dst, const dsTSTRING_flat *src);   /* dsTSTRING<char>::operator= (shares buffer) */
extern "C" int  strcmp(const char *a, const char *b);

/* --- level timer (real, DB-verified type; only ->timeLife is read here) --- */
#include "../ws/gs/gsLVL_TIMER.h"

extern void apLogSetDumpRightNowMode(int enable);
extern void _apDumpLog(const char *file, const char *fmt, ...);

typedef struct vidLOCK vidLOCK;
extern vidLOCK vidLock;         /* the global critical section guarding the video driver */
extern struct osLOCK hcexHaloLogic;   /* the global critical section guarding the legacy Blam logic thread;
                                          lockDepth lives at its head (see hcex_wait_jobs.c) */
extern void vidLOCK_Lock(vidLOCK *self, const char *file, int line);
extern void vidLOCK_Unlock(vidLOCK *self, const char *file, int line);
extern void osLOCK__Lock(struct osLOCK *self, const char *file, int line);
extern void osLOCK_Unlock(struct osLOCK *self, const char *file, int line);

typedef struct jbmMANAGER jbmMANAGER;
typedef struct jbmJOB jbmJOB;
extern jbmMANAGER gsJobManager;
extern void jbmMANAGER_AddJob(jbmMANAGER *self, unsigned long threadMask, jbmJOB *job);
extern jbmJOB hcex_job_legacy_render;

extern void osPIXBeginEvent(const char *string);
extern void osPIXEndEvent(void);

/* apCOUNTER_TIME_CALL profiling scopes (see the file-level note above). */
extern void cnt_hcex_process_frame_scope_enter(void);
extern void cnt_hcex_process_frame_scope_exit(void);
extern void cnt_hcex_main_loop_enter(void);
extern void cnt_hcex_main_loop_exit(void);
extern void cnt_hcex_fp_sync_enter(void);
extern void cnt_hcex_fp_sync_exit(void);
