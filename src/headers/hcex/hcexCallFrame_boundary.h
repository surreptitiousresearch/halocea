#pragma once
/* Boundary declarations for hcexCallFrame.cpp.
 *
 * hcexCallFrame is the ws-engine per-frame dispatch entry point the Blam bridge installs as
 * gsMsgCallFrame's frame callback: it runs the think-callback pass, the pre-process-frame job,
 * the render-prep/push/merge job graph (see CLAUDE.md's "Frame Job Pipeline" table, modeled in
 * gsFRAME_JOB_PIPELINE.h), then the post-process-frame job. Every large ws-engine subsystem it
 * merely calls into without touching fields (particle system, texture-load package, task
 * scheduler, think-callback registry, usable/logger manager, streaming-request manager) is an
 * extern boundary per the project's re-source conventions — declared as an opaque type plus
 * free-function wrappers for its virtual calls; only srMANAGER's `isCopyDelayedCalled` bit
 * (written directly by hcexCallFrame) is modeled as a real field. */

#include "../ws/ap/apSTATE_T.h"

/* --- opaque boundary classes (methods called, layout not needed) --- */
struct gsPARTICLE_SYS;
namespace package { struct GS_PACKAGE; }
namespace gts { struct SYSTEM; }
namespace ds { template<class F> struct CALLBACK_MNG; }
struct useLOGGER_MNG;
struct vidDRIVER;

/* --- srMANAGER: only the isCopyDelayedCalled/isAnyQueryAdded bits are touched directly here --- */
#include "../ws/ds/dsVECTOR.h"
struct jbmJOB;   // boundary — frame-job record

/* srMANAGER used to have a second file-scope body right here. The full DB layout
 * (types_members srMANAGER, DB size 32) it carried is now the single definition, and lives in
 * ws/gs/gsSR_MANAGER.h beside gsSR_MANAGER (the concrete manager) and `pSrMng`, which is where the
 * rest of the family is declared. That header also owns srMANAGER_vtbl's forward declaration and
 * the RunAllDelayedJobs / ShootRay declarations the other half of the pair contributed. */
#include "../ws/gs/gsSR_MANAGER.h"

/* --- free-function boundary wrappers for virtual/boundary method calls --- */
extern void package_GS_PACKAGE_ProcessLoadTextures(package::GS_PACKAGE *self);
extern void gts_SYSTEM_Think(gts::SYSTEM *self);
extern void ds_CALLBACK_MNG_void_ProcessAll(ds::CALLBACK_MNG<void (__cdecl *)(void)> *self);
extern bool useLOGGER_MNG_IsGamePaused(useLOGGER_MNG *self);
extern void gsPARTICLE_SYS_PushDelayedCDT(gsPARTICLE_SYS *self);
extern void srMANAGER_ProcessLastPortion(srMANAGER *self);      // return value discarded by caller
extern void srMANAGER_CheckAddExecJobs(srMANAGER *self);
extern void srMANAGER_CopyDelayed2LastValid(srMANAGER *self);
extern void vidDRIVER_Suspend(vidDRIVER *self);                 // virtual
extern void vidDRIVER_BindToProcessor(vidDRIVER *self, int threadProcessorNmb);

/* --- globals --- */
// gsRENDER_SYSTEM — full DB layout lives in ws/gs/gsRENDER_SYSTEM.h (skipFrameNmb@48 read here).
#include "../ws/gs/gsRENDER_SYSTEM.h"
extern gsRENDER_SYSTEM *gsSysRender;

extern gsPARTICLE_SYS        *gsSysParticle;
extern package::GS_PACKAGE    *gs_pkg;              // package::gs_pkg
extern gts::SYSTEM             gts_taskSystem;       // gts::taskSystem
extern ds::CALLBACK_MNG<void (__cdecl *)(void)> thinkCallbacks;
extern useLOGGER_MNG          *useLoggerMng;
extern srMANAGER              *pSrMng;
extern vidDRIVER              *vidDriver;

extern bool          ui_new_g_bLoadingScreenActive; // ui_new::g_bLoadingScreenActive
extern unsigned int  gsAppState;
extern unsigned int  gsAppState2;
extern unsigned int  gsMsgMTConfig;
extern int           hcex_cur_render_mode;
extern bool          hcex_enable_pix;
extern bool          hcex_obj_sync_matrices_in_progress;
extern int           colorizePerFrameNmb;
extern int IGNORE_STRONG_ASSERT; /* .data @0x841DB148 - ?IGNORE_STRONG_ASSERT@@3HA (def: src/data/IGNORE_STRONG_ASSERT.cpp) */

/* --- misc free-function boundaries --- */
extern void ProcessSocketsNotifications(void);
extern void dsUpdateEventMgr(void);
extern void dsSTRID_UpdateThread(void);
extern void _gsMsgCalcElapsedTime(void);
extern int  gsMsgIsMode(unsigned int mode);
extern int  apProcLock_GetGameLogicThread(void);
extern void apProcLock_SetGameLogicThread(int threadId);
extern void hcex_obj_sync_matrices(void);
#include "../ws/dbg/STRONG_ASSERT_DUMMY.h" // the only Crash is a C++ member, not a C symbol
