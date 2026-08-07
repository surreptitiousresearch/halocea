#pragma once
#include "../msg/msgADDR.h"
#include "../ap/apSTATE_T.h"
#include "../m3d/m3dMATR.h"
#include "../ps/psSECTION.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsAFFIX_STRING.h"
#include "../anim/animINST.h"
// ws-engine gs: level-system message address — owns the current-level scene/DB pathing,
// preload state, and anchor transform, and reacts to obj/animINST/template lifecycle
// notifications delivered by the anim and obj subsystems while a level is loaded.
// DB-verified layout (types_members gsLVL_SYSTEM) — size 644 (0x284).
// Leading base (offset 0, size 32) is msgADDR: modeled as public inheritance per convention.

struct gsLVL_DATA;         // gsLVL_DATA.h — level scene-data buffer handle (pointer only)   boundary
struct gsLOAD_SCENE_PARAM; // gs subsystem — level load parameters (pointer only)            boundary
struct objOBJ;              // ../obj/objOBJ.h — model object (pointer only)                  boundary
struct objLOAD_NOTIFY_DATA; // obj subsystem — object-load notify payload (pointer only)      boundary
struct animTPL;              // ../anim/animTPL.h — animation template (pointer only)         boundary

// DB-verified dispatch vtable (types_members gsLVL_SYSTEM_vtbl). Slots 0..36 are the
// inherited msgADDR_vtbl slots (re-listed here since gsLVL_SYSTEM overrides them with its
// own `this` type); slots 40+ are gsLVL_SYSTEM's own virtuals.
struct gsLVL_SYSTEM; /* fwd: vtbl slots take gsLVL_SYSTEM* */
typedef struct gsLVL_SYSTEM_vtbl {
    dsTYPE_ID   *(*TypeID)(gsLVL_SYSTEM *self, dsTYPE_ID *result);                      // 0x00
    void         (*dtr_gsLVL_SYSTEM)(gsLVL_SYSTEM *self, int deleteFlag);                               // 0x04 — deleting dtor: vftable+0x04 holds ??_EgsLVL_SYSTEM@@UAAPAXI@Z
    msgRES       (*ProcessMsg)(gsLVL_SYSTEM *self, int msg, void *pInfo, msgADDR *pSend);      // 0x08
    int          (*PreProcessMsg)(gsLVL_SYSTEM *self, int msg, void *pInfo, msgADDR *pSend);   // 0x0C
    int          (*PostProcessMsg)(gsLVL_SYSTEM *self, int msg, void *pInfo, msgADDR *pSend);  // 0x10
    int          (*GetPriorityShift)(gsLVL_SYSTEM *self);                               // 0x14
    unsigned int (*GetSuspendMask)(gsLVL_SYSTEM *self);                                 // 0x18
    void         (*OnChangeMode)(gsLVL_SYSTEM *self, unsigned int, unsigned int);       // 0x1C
    void         (*OnSuspend)(gsLVL_SYSTEM *self, int);                                 // 0x20
    void         (*NotifyPostTermMsg)(gsLVL_SYSTEM *self, msgADDR *);                   // 0x24
    int          (*LoadLevel)(gsLVL_SYSTEM *self, gsLOAD_SCENE_PARAM *);                // 0x28
    void         (*DestroyLevel)(gsLVL_SYSTEM *self, gsLOAD_SCENE_PARAM *);             // 0x2C
    animTPL     *(*FindTplName)(gsLVL_SYSTEM *self, const char *, dsAFFIX_STRING *, int);      // 0x30
    void         (*ObjLoadNotify)(gsLVL_SYSTEM *self, objOBJ *, objLOAD_NOTIFY_DATA *);        // 0x34
    void         (*InstLoadNotify)(gsLVL_SYSTEM *self, animINST *);                     // 0x38
    void         (*ObjDestroyNotify)(gsLVL_SYSTEM *self, objOBJ *);                     // 0x3C
    void         (*InstDestroyNotify)(gsLVL_SYSTEM *self, animINST *);                  // 0x40
    void         (*TplDestroyNotify)(gsLVL_SYSTEM *self, int);                          // 0x44
    void         (*TplDestroyNotify_2)(gsLVL_SYSTEM *self, animTPL *);                  // 0x48
    int          (*GetLevelId)(gsLVL_SYSTEM *self, dsTSTRING<char>);                    // 0x4C
    int          (*GetCurLevelId)(gsLVL_SYSTEM *self);                                  // 0x50
    void         (*ProcessLEVEL_LOADED)(gsLVL_SYSTEM *self);                            // 0x54
    int          (*ProcessINIT_LEVEL)(gsLVL_SYSTEM *self);                              // 0x58
    void         (*ProcessTERM_LEVEL)(gsLVL_SYSTEM *self);                              // 0x5C
    void         (*ProcessFRAME)(gsLVL_SYSTEM *self);                                   // 0x60
} gsLVL_SYSTEM_vtbl;

struct gsLVL_SYSTEM : public msgADDR {
    gsLVL_DATA         *pSLData;         // 0x020 current level's scene-data buffer handle
    int                 isSLDataLoaded;  // 0x024
    int                 haveLightZones;  // 0x028
    apSTATE_T<unsigned long> stateLvl;   // 0x02C level-system state bitmask
    char                pathScn[260];    // 0x030 current level scene path
    char                pathDB[260];     // 0x134 current level DB path
    dsTSTRING<char>     nameAnchor;      // 0x238
    m3dMATR             matrAnchor;      // 0x23C anchor transform
    psSECTION           psPreloadList;   // 0x27C
    dsTSTRING<char>     nameScnCur;      // 0x280 name of the scene currently loaded

    // ---- reversed in the ws_gs_0001 batch ----
    // 0x82739F48 — post-load-init housekeeping for a newly-created animINST: LOD table
    // init, drop the model's non-runtime-needed data, and pick a render pass by state.
    void HandleAnimInstBeforeMSG_INIT(animINST *pInst);
    // 0x8273B278 — after an animINST finishes its INIT message: flag shadow-cast state
    // from a lighting-template affix, then notify the level system's InstLoadNotify chain.
    void HandleAnimInstAfterMSG_INIT(animINST *pInst);

    // ---- same-class / vtable methods called but external to this batch ----
    void InstLoadNotify(animINST *pInst); // 0x8273BAB8 — virtual (gsLVL_SYSTEM_vtbl slot 0x38)  boundary
    // virtual (gsLVL_SYSTEM_vtbl slot 0x30) — look up an anim template by name, applying
    // instAff's affix override; noLoad suppresses an on-demand load if the template isn't
    // resident. Called by gsExtFindTplName.
    // 0x827986D0 — reversed in gsLVL_SYSTEM__FindTplName.cpp.
    animTPL *FindTplName(const char *name, dsAFFIX_STRING *instAff, int noLoad);

    // ---- same-class helpers/callbacks invoked by FindTplName (bodies external to this batch) ----
    // Static load-callback hooks (take the template + owning level system). boundary
    static void TplStartLoadCB(animTPL *pTpl, gsLVL_SYSTEM *self);
    static int  TplFinishLoadCB(animTPL *pTpl, gsLVL_SYSTEM *self);
    // Object-load callback installed via sioSetObjLoadCB during on-demand template load. boundary
    static void ObjLoadCB();
    // Kick the actual template load from disk into `pTpl`. boundary
    int  LoadTpl(const char *name, animTPL *pTpl, int flags);
    // Finalize a loaded template's materials/affix binding; false on failure. boundary
    int  _FinishTplLoadMtl(animTPL *pTpl, dsAFFIX_STRING *instAff);
};
