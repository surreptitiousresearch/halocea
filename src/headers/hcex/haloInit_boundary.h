#pragma once
/* Boundary declarations for haloInit.cpp, beyond what's already modeled in
 * engine_control/haloENGINE_CONTROL_boundary.h (sslCLASS_REF/sslSYSTEM/sslOBJ_REF/apCONFIG/
 * STRONG_ASSERT_DUMMY/IGNORE_STRONG_ASSERT/gCfg/load_as_halo/player_spawn_count/hcex_split_screen,
 * all reused directly). Everything here is a ws-engine subsystem haloInit only registers with or
 * calls into — declared per the project's re-source conventions, not descended into. */

#include <stdint.h>
#include "engine_control/haloENGINE_CONTROL_boundary.h"
#include "../ws/dsc/dscDESC_FAMILY.h"
#include "../ws/ia/iaIACTOR.h"
#include "../ws/inst/instMANAGER_HANDLER.h"
#include "HALO_MNG.h"
#include "HALO_MNG_DESC.h"
#include "hcexHALO_CUSTOM_OBJ_LIST.h"
#include "hcexPARTICLE_SYS.h"
#include "hcex_preproc_frame_boundary.h" // ENGINE_CONTROL*, gsEngineCtrl, ENGINE_CONTROL_SetSplitScreen
#include "../ws/prop/propHALO_OBJ.h"
#include "../cheats.h"

extern void *operator new(size_t size, const char *file, int line);
struct dscBRAND_vtbl; // boundary — dsc subsystem
template<class R, class T> R *dsMakeFunc(const char *file, int line); // boundary — factory-thunk template
class entLIGHT;      // boundary — ws-engine entity subsystem (extern per re-source conventions)
class entLIGHT_DESC; // boundary — ws-engine entity subsystem (extern per re-source conventions)

/* --- ent (entity handler registry): only AddHandler is called --- */
#include "../ws/ent/entMANAGER_HANDLER.h" // full entMANAGER_HANDLER (AddHandler param base type)
#include "hcexHALO_MODEL.h"   // full hcexHALO_MODEL (: hcexHALO_OBJECT : entMANAGER_HANDLER)
#include "hcexHALO_OBJECT.h"  // full hcexHALO_OBJECT (: entMANAGER_HANDLER)
struct entMANAGER_ELEMENT; // ws/ent — 68-byte registry element; boundary (pointer-free vector storage)
typedef struct entMANAGER {
    dsVECTOR<entMANAGER_ELEMENT, 8> mngList; // 0x00 — DB-verified (types_members entMANAGER)

    void AddHandler(char *nameClass, entMANAGER_HANDLER *pHandler); // 0x826DCD08 — boundary body
} entMANAGER;
extern entMANAGER entManager;

extern hcexHALO_MODEL  hcexHaloModel;
extern hcexHALO_OBJECT hcexHaloObject;

/* ssl script system (GetGlobalObj/GetClass called to register the Halo callbacks) + render hook. */
extern sslSYSTEM *gsSslSystem;
extern void hcexRenderInit();

/* --- dsc RTTI family (abstract-brand registration; used for the propHALO_OBJ base brand) --- */
#include "../ws/dsc/dscMAKE_FAMILY.h"
/* DB-verified hierarchy: dscRTTI_FAMILY : dscMAKE_FAMILY (: dscDESC_FAMILY) + typeSys@140. */
typedef struct dsTYPE_ID_SYS {
    int curTypeNmb; // 0x00 — DB-verified (types_members dsTYPE_ID_SYS)
} dsTYPE_ID_SYS;
struct dscRTTI_FAMILY : dscMAKE_FAMILY {
    dsTYPE_ID_SYS typeSys; // 0x8C (140)

    template<class T> dscBRAND *RegisterAbstractBrand(const dsTSTRING<char> &name, const dsTSTRING<char> &nameParent); // boundary
};
struct iaPROP_FAMILY : dscRTTI_FAMILY {}; // boundary — no extra fields modeled
extern iaPROP_FAMILY *pPropFamily;

/* --- ia family (interactive-actor family; owns iaFamily, used to spawn HALO_MNG) --- */
struct iaFAMILY : dscRTTI_FAMILY { /* DB-verified base: dscRTTI_FAMILY */
    template<class T> T *CreateIA(const dsTSTRING<char> &nameBrand); // boundary
};
extern iaFAMILY *iaFamily;

/* --- gsPARTICLE_SYS / instance-manager-handler swap-in for Halo mode --- */
extern gsPARTICLE_SYS *gsSysParticle;
extern instMANAGER_HANDLER *instManager;

/* --- misc globals/functions touched only here --- */
extern float gsElapsedTimeMin;
extern void (*UI_EXT_RENDER)();
extern void  hcex_render_hud();
extern "C" const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */

extern "C" {
    void hcex_library_init();
    void hcex_cine_init();
    void InputQueueInit();
    void haloEngineCtrlInit();
    void hcex_effect_names_parse();
    void haloInitC();
    int  main_loop_init1();
    void main_set_game_connection_to_film_playback(int filmToken);
    int  hcex_allow_saved_film_recoring;
}

/* dbgVAR / dbgVAR_bool — DB-verified layout (types_members dbgVAR, dbgVAR_SIMPLE<bool,1>):
 * vtable@0, name@4 (dsTSTRING<char>), typeId@8 (int) -- 12 bytes base; value@12 (bool) on the
 * dbgVAR_SIMPLE<bool,1> specialization used here ("ui/ingame text/enable"). */
#include "../ws/wb/dbgVAR_boundary.h" /* canonical dbgVAR / dbgVAR_IMPL<T,N> */
/* odr_dup drain: a `struct dbgVAR_BOOL : dbgVAR_IMPL<bool, 1> {};` used to sit here, duplicating
 * the body in d3d_render_state_boundary.h. Nothing reachable from this header ever named it —
 * haloInit.cpp casts to `dbgVAR_bool` (below), the DB-verified spelling of dbgVAR_SIMPLE<bool,1> —
 * so it is simply deleted rather than re-homed. */
/* dbgVAR_bool has a single definition in hcex_dbgvar_boundary.h — include it rather than a local
 * typedef, so a TU pulling both headers doesn't hit a dbgVAR_bool redefinition (C2371). */
#include "hcex_dbgvar_boundary.h"
struct dbgVAR_MANAGER;
extern dbgVAR_MANAGER *dbgVAR_MANAGER_GetManager();
extern dbgVAR *dbgVAR_MANAGER_FindFirst(dbgVAR_MANAGER *mgr, const dsTSTRING<char> *name); // boundary
extern void     dbgVAR_MANAGER_SendVarUpdate(dbgVAR_MANAGER *mgr, dbgVAR *var);             // boundary
extern "C" uint8_t      debug_no_drawing;
extern void      vidLOCK_Lock(void *lock, void *site, int flag);   // boundary
extern void      vidLOCK_Unlock(void *lock, void *site, int flag); // boundary
extern void     *vidLock;

extern haloENGINE_CONTROL *haloEngineCtrl;
extern void ENGINE_CONTROL_SetSplitScreen(void *self, int isSplit); // boundary
extern void haloENGINE_CONTROL_InitSettings(haloENGINE_CONTROL *self);

/* DB type VOID_CALLBACK_MNG (== ds::CALLBACK_MNG<void(*)(void)>): callbacks vector @0. */
typedef void (*hcex_frame_callback)(void);
typedef struct VOID_CALLBACK_MNG {
    dsVECTOR<void (*)(void), 8> callbacks; // 0x00 — DB-verified (types_members VOID_CALLBACK_MNG)

    void PushBack(hcex_frame_callback cb); // boundary
} VOID_CALLBACK_MNG;
typedef VOID_CALLBACK_MNG onPreProcessFrameCallbacks;
extern VOID_CALLBACK_MNG onPreProcessFrame;
extern void hcex_preproc_frame();

extern "C" void cbSetHaloCamActive(sslOBJ_REF self, int argc, dsDATA *argv, dsDATA *retVal, sslOBJ_REF caller);
extern "C" void cbHcexTeleportToCam(sslOBJ_REF self, int argc, dsDATA *argv, dsDATA *retVal, sslOBJ_REF caller);
extern "C" void cbMakeAutosave(sslOBJ_REF self, int argc, dsDATA *argv, dsDATA *retVal, sslOBJ_REF caller);
extern "C" void cbIsHaloMode(sslOBJ_REF self, int argc, dsDATA *argv, dsDATA *retVal, sslOBJ_REF caller);
