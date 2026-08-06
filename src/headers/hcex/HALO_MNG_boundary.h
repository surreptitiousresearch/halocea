#pragma once
/* Boundary declarations for HALO_MNG::ProcessFRAME / ProcessMsg / ProcessTERM_LEVEL -- globals and
 * free functions from other subsystems (Blam bridge, dbg console, snd) that these three methods
 * call or read but that are not part of this batch's target classes. All addresses/types are
 * DB-verified (funcs / names / types_members); nothing here is invented. */

/* Uses the REAL (non-boundary-shim) ws-engine ds headers throughout -- HALO_MNG.cpp is true C++,
 * unlike the plain-C hcex bridge files that consume hcex_ds_boundary.h's simplified dsSTRID /
 * dsTSTRING / dsEVENT_MGR shims, so those shims are intentionally NOT included here to avoid a
 * duplicate-definition clash with the real classes below. */
#include "../ws/ds/dsSTRID.h"
#include "../ws/ds/dsTSTRING.h"
#include "../ws/ds/dsEVENT_MGR.h"
#include "../ws/ds/dsPARAM_LIST.h"
#include "../ws/ds/dsPARAM.h"
#include "../ws/wb/dbgVAR_boundary.h"  /* dbgVAR_IMPL<T,N> */
#include "../ws/dbg/dbgVAR_MANAGER.h"
#include "../ws/vid/vidLOCK.h"
#include "../ws/snd/SYSTEM_CUSTOM.h"
#include "../ws/ds/dsVECTOR.h"
#include "../ws/ds/dsPAIR.h"
#include "../ws/scn/scnINST_CREATE_DATA.h"
#include "../cheat_globals.h"

extern "C" void dlFree(void *ptr);
extern dsEVENT_MGR *gEventMgr;

// dsSTATIC_PARAM_LIST<2> -- a fixed 2-slot dsPARAM_LIST, built directly on the real dsPARAM_LIST/
// dsPARAM above (DB-verified layout: dsPARAM_LIST base@0 (8) + dsPARAM storage[2]@8 (24) -- 32
// bytes). Only the single Add<dsTSTRING<char>> overload HALO_MNG::ProcessFRAME uses is modeled;
// the many other instantiations (Add<int>, Add<bool>, ...) are boundary/not needed here.
template<int N>
struct dsSTATIC_PARAM_LIST : dsPARAM_LIST {
    dsPARAM storage[N]; // 0x08

    dsSTATIC_PARAM_LIST();   // 0x823D0788 (N=2) -- boundary
    ~dsSTATIC_PARAM_LIST();  // 0x823D0720 (N=2) -- boundary

    // 0x823DB2A8 -- Add<dsTSTRING<char>>(const char *id, const dsTSTRING<char>&): interns `id`
    // then appends {id, data} to storage and grows `length`. Extern boundary per re-source
    // conventions. (A sibling dsSTRID-keyed overload also exists at 0x823D84D8; this
    // is the one HALO_MNG::ProcessFRAME actually calls, passing string literals directly.)
    void Add(const char *id, const dsTSTRING<char> &value);
};

typedef dsSTATIC_PARAM_LIST<2> dsSTATIC_PARAM_LIST_2; /* 32 bytes -- DB dsSTATIC_PARAM_LIST<2> */

/* --- dbg console-variable globals (dbgVAR_IMPL<T,N>, DB-verified instantiations) --- */
extern "C" dbgVAR_IMPL<bool, 5>            dbg_forceReload;    /* 0x841C3A04 */
extern "C" dbgVAR_IMPL<dsTSTRING<char>, 4> dbg_defaultLevel;    /* 0x841C4070 */
extern "C" dbgVAR_IMPL<dsTSTRING<char>, 4> dbg_defaultChpt;     /* 0x841C41F0 */

/* --- misc HCEX/frame-loop globals --- */
extern dsVECTOR<dsTSTRING<char>, 8> hcexUsedSfx;            /* 0x841C42AC */
extern char                        *gsSceneFullName;        /* 0x841CAE88 */
extern bool                         hcex_call_frame;        /* 0x841C38E2 */
extern bool                         hcex_first_load;         /* 0x841370B8 */
extern bool                         hcex_enable_pix;         /* 0x841370B9 */
extern bool                         hcex_inside_processframe;/* 0x841C38E1 */
extern vidLOCK                       vidLock;                 /* 0x84141014 */

/* opaque: instMANAGER_HANDLER is an extern boundary per re-source conventions. Only used via a reinterpret_cast to
 * a dsVECTOR<dsPAIR<unsigned long,scnINST_CREATE_DATA>,8>* at &instManager[1], matching the
 * decompiler exactly -- boundary. */
struct instMANAGER_HANDLER;
extern instMANAGER_HANDLER *instManager; /* 0x8427C128 */

/* --- Blam bridge / main-loop functions (unmangled, extern "C"-style Blam symbols) --- */
extern "C" {
    int  main_loop_init_console(void);   /* 0x83688C28 */
    int  main_start_time(void);          /* 0x8368AB48 */
    int  main_stop_time(void);           /* 0x8368AB28 */
    int  main_loop_init2(void);          /* 0x8368B938 */
    int  main_loop_update(void);         /* 0x8368B9F0 */
    void main_coop_first_update(void);   /* 0x823BB3C8 */
    int  rasterizer_recreate_vbuf(void); /* 0x836A5FF0 (see src/rasterizer_recreate_vbuf.c) */
}

/* hcex_cam_sync @ 0x8369E640 -- same address as the already-reversed src/hcex/hcex_cam_sync.c
 * (void hcex_cam_sync(void)). IDA's funcs table shows a default-guessed
 * `__int64 hcex_cam_sync(_DWORD)` signature for this unmangled symbol, which is what leaks into
 * HALO_MNG::ProcessMsg's decompiler output as a phantom argument/64-bit return -- the real,
 * previously-reversed body takes no arguments and returns nothing. Declared void(void) here to
 * match. */
extern "C" void hcex_cam_sync(void);

/* hcex_obj_sync_matrices @ 0x823DD8C0 -- mangled "?hcex_obj_sync_matrices@@YAXXZ" demangles to
 * void(void); the funcs table's associated `prototype` column (8 ints) is a stale/incorrect type
 * override -- the mangled name is ground truth, and the ProcessMsg call site loads no registers
 * before the `bl`, confirming a bare no-arg call. */
extern "C" void hcex_obj_sync_matrices(void);

extern "C" {
    void hcex_process_frame(void);          /* 0x823E8A40 */
    void hcex_update_anim(float dt);         /* 0x823DE8B0 */
    void hcex_cine_update(void);             /* 0x823B91E8 */
    void hcex_pix_begin_event(const char *text, ...); /* 0x823CED08 */
    void osPIXEndEvent(void);                /* 0x825E5738 */
    void hcex_fp_models_term(void);          /* 0x823DDC50 */
    void hcex_term_breakable_surfaces(int clear); /* 0x823B2608 */
    void hcex_destroy_dead_intances(void);   /* 0x823EDF08 */
    void hcex_obj_term_instances(void);      /* 0x823EDF90 */
    void hcex_cine_term_level(void);         /* 0x823B5420 */
}
