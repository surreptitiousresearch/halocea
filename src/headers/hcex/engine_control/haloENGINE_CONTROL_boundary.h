#pragma once
/* Boundary declarations shared by the haloENGINE_CONTROL / EVENT_DISP_haloENGINE_CONTROL method
 * bodies in src/hcex/engine_control/. These are ws-engine/Blam subsystems haloENGINE_CONTROL only
 * calls into (never touches fields of, apart from what's modeled below) — declared per the
 * project's re-source conventions, not descended into. DB-verified signatures throughout (funcs /
 * types_members); anything not address-verified in this batch is marked boundary. */

#include <stdint.h>
#include "../../ws/ds/dsTSTRING.h"
#include "../../ws/ds/dsPARAM_LIST.h"
#include "../../ws/ds/dsEVENT.h"
#include "../../ws/ds/dsDATA.h"
#include "../../ws/ds/REF_TYPE.h"
#include "../../ws/ds/dsREF_VECTOR.h"
#include "../../ws/ds/dsVECTOR.h"
#include "../../ws/ds/dsEVENT_MGR.h"
#include "../../ws/ssl/sslCLASS_REF.h"
#include "../../ws/ssl/sslOBJ_REF.h"
#include "../../ws/ssl/sslSYSTEM.h"
#include "../../ws/ssl/sslCB_HOST.h"
#include "../../ws/ap/apPROCESSOR_LOCK.h"
#include "../../apCL.h"
#include "../haloPLAYER_ACTION.h"
#include "../haloRAW_INPUT_X360.h"
#include "../../cheats.h"
#include "../../player_profile.h"
#include "../ssl_exporter/SSL_EXPORTER_haloENGINE_CONTROL.h"

/* --- CRT / allocator --- */
typedef struct _player_ui_globals _player_ui_globals;
extern "C" void dlFree(void *ptr);
extern "C" int  strcmp(const char *a, const char *b);
extern void _apLog(const char *format, ...); // ?_apLog@@YAXPBDZZ
extern "C" int  osFileIsExist(const char *path);
extern "C" int sprintf_0(char *string, const char *format, ...);

/* --- ws-engine dsTSTRING<char> free-function boundary (not yet members of dsTSTRING_flat.h) --- */
// operator+(dsTSTRING<T>, const char*) is provided canonically by ws/ds/dsTSTRING.h; a duplicate
// here made `str + "literal"` ambiguous, so it is dropped.

/* fnmGetName / fnmGetNameStr (fname.h path-splitting helpers) — boundary. */
extern void             fnmGetName(const char *path, char *out /* [272] */);
extern dsTSTRING<char> *fnmGetNameStr(dsTSTRING<char> *out, const dsTSTRING<char> *path);

extern haloRAW_INPUT_X360 hcex_coop_u1_inp_remote;
extern dsEVENT_MGR       *gEventMgr;
extern sslSYSTEM         *gsSslSystem;   /* global ssl script system (AddClass/GetGlobalObj) */

/* --- STRONG_ASSERT_DUMMY::Crash (0x825202A8, ?Crash@STRONG_ASSERT_DUMMY@@QAAXPBD0H0@Z) --- */
#include "../../ws/dbg/STRONG_ASSERT_DUMMY.h" /* canonical (member + static Crash overloads) — avoids C2011 */
extern int IGNORE_STRONG_ASSERT; /* .data @0x841DB148 - ?IGNORE_STRONG_ASSERT@@3HA (def: src/data/IGNORE_STRONG_ASSERT.cpp) */

/* --- apCONFIG (full DB layout lives in ws/ap/apCONFIG.h) --- */
#include "../../ws/ap/apCONFIG.h"
#include "../../ws/ds/LIST.h"
#include "../../ws/ds/dsSMART_PTR.h"
#include "../../ws/ds/dsPAIR.h"
/* apCONFIG member templates GetValue<T> / Retrieve<T,D> are boundary
 * (??$GetValue@...@apCONFIG@@QBA_NPBDAA...@Z / ??$Retrieve@...@apCONFIG@@QBAXPBDAAV1@ABV1@@Z);
 * the reversed struct itself is in apCONFIG.h. */

/* --- gsUSER_CFG_MNG / gsUSER_MNG — canonical full DB layouts (types_members) with the touched
 * methods. gsUSER_CFG_MNG.h pulls gsUSER_MNG.h, which also declares `extern gsUSER_MNG *gsUserMng;`
 * plus PrimaryUserIdx()/SecondaryUserIdx(); gsUSER_CFG_MNG.h additionally carries the no-arg
 * Get() overload @0x823D29E0 that this copy lacked. --- */
#include "../../ws/gs/gsUSER_CFG_MNG.h"

extern apCONFIG        *gCfg;
extern gsUSER_CFG_MNG   *gsUserCfgMng;

extern "C" _player_ui_globals * hcex_get_local_player_profile(int16_t local_player_index);
extern "C" void set_local_player_controls_from_player_profile(int16_t local_player_index);

/* --- gsINP_SYSTEM (full DB layout lives in ws/gs/gsINP_SYSTEM.h); gsiCONTEXT expanded here from
 * types_members gsiCONTEXT (its own ws header is a forward declaration only) --- */
#include "../../ws/gs/gsINP_SYSTEM.h"
#include "../../ws/ds/dsSTRID.h"
#include "../../ws/ds/MAP.h"

// gsINPUT_LAYOUT / gsiFORCE_FEEDBACK / gsiCONTEXT aggregate block relocated to its
// canonical ws-side home:
#include "../../ws/gs/gsiCONTEXT.h"
extern gsINP_SYSTEM *gsSysInput;
extern "C" void OnInputUpdateCB();
extern "C" bool isInputSwitchedOff;
extern "C" void clear_hcex_input_x360();

/* --- co-op session plumbing (SESSION_BASE / net::RELAY — large net-layer classes, referenced
 * only by pointer here; kept as forward declarations with the two touched methods declared in
 * free-function form (compiler-generated thiscall). --- */
namespace net { struct RELAY; }
struct SESSION_BASE;
struct GS_SESSION_SCRIPT_DISPATCHER; // boundary

// ?GetRelay@SESSION_BASE@@QAAPAVRELAY@net@@XZ — boundary
extern net::RELAY *SESSION_BASE_GetRelay(SESSION_BASE *self);
// ?GetLocalClientId@RELAY@net@@QBA?AW4CLIENT_ID@2@XZ — const, returns net::CLIENT_ID by value.
// Modeled as a plain int (the enum's underlying representation). — boundary
extern int net_RELAY_GetLocalClientId(const net::RELAY *self);

struct SESSION_MANAGER_vtbl; // boundary — vtable shape
// types_members SESSION_MANAGER: __vftable@0, m_iSessionID@4, m_vecSessions@8,
// m_vecSessionDispatchers@28 — size 48.
typedef struct SESSION_MANAGER {
    SESSION_MANAGER_vtbl                 *__vftable;    // 0x00
    int                                   m_iSessionID; // 0x04
    dsVECTOR<SESSION_BASE *, 8>           m_vecSessions;// 0x08
    dsVECTOR<dsPAIR<SESSION_BASE *, GS_SESSION_SCRIPT_DISPATCHER *>, 8> m_vecSessionDispatchers; // 0x1C
} SESSION_MANAGER;
extern SESSION_MANAGER *gSessionManager;
extern "C" void coop_input_initialize(SESSION_BASE *pSession);

/* --- dbgVAR_MANAGER (full DB layout + SetDbgVar/GetManager live in ws/dbg/dbgVAR_MANAGER.h) --- */
#include "../../ws/dbg/dbgVAR_MANAGER.h"

/* --- fnmFILE_LOOK_THROUGH (map-cycle file enumeration; only Find() touched).
 * types_members fnmFILE_LOOK_THROUGH: __vftable@0 — size 4. --- */
struct fnmFILE_LOOK_THROUGH_vtbl; // boundary — vtable shape
typedef struct fnmFILE_LOOK_THROUGH {
    fnmFILE_LOOK_THROUGH_vtbl *__vftable; // 0x00

    fnmFILE_LOOK_THROUGH();
    ~fnmFILE_LOOK_THROUGH();
    // ?Find@fnmFILE_LOOK_THROUGH@@QAA_NV?$dsTSTRING_flat@D@@_N@Z — locate the first (and each subsequent,
    // if re-invoked) file matching `mask`; returns whether a match was found.
    bool Find(dsTSTRING<char> mask, bool processSubfolders); // boundary
} fnmFILE_LOOK_THROUGH;

/* --- Blam bridge globals/functions touched by haloENGINE_CONTROL (per CLAUDE.md's Blam Bridge
 * table; these are the legacy engine's own entry points, called through as extern boundaries) --- */
extern "C" {
    extern int  load_as_halo;
    extern int  firstLoadInSolo;
    extern int16_t  player_spawn_count;
    extern bool hcex_call_frame;   // DB: ?hcex_call_frame@@3_NA (_N = bool), not int
    extern bool hcex_split_screen; // DB: ?hcex_split_screen@@3_NA (_N = bool), not int
    extern int  hcex_coop_online_test_local;
    extern int  hcex_coop_local_player_index;
    extern int  hcex_off_vibration;
    extern char *hcex_chpt_ext;
    extern int  hcex_load_checkpoint;

    /* dbg toggle — real type dbgVAR_SIMPLE<bool,1> (applied_types @0x841D906C); declared below,
     * outside this extern "C" block, via the ws dbgVAR boundary header. */

    extern int  verify_checkpoint(const dsTSTRING<char> *checkpoint);
    extern void hcex_game_state_set_saved_game_valid(int isValid);
    extern int  hcex_coop_get_player_action(haloPLAYER_ACTION *hcex_actions);
    extern void hcex_coop_apply_player_actions(haloPLAYER_ACTION *hcex_actions);
    extern void hcex_raw_input_get(int gamepadId, haloRAW_INPUT_X360 *inp);
    extern void input_set_gamepad_vibrater_state(int userId, int leftMotor, int rightMotor);
    extern void game_time_set_paused(unsigned char paused);

    extern int  main_revert_map();
    extern void main_set_difficulty(short difficulty);
    extern void main_set_map_name(const char *name);
    extern int  main_defer_map_map_change();
    extern int  main_goto_main_menu();
    extern unsigned char ui_widget_event_handler_function_invoke(void *widget, void *event, short function, unsigned char *widget_deleted);
}

#include "../../ws/wb/dbgVAR_boundary.h"
extern "C" dbgVAR_SIMPLE<bool, 1> dbg_hcex_test_online_coop_local; /* applied_types: dbgVAR_SIMPLE<bool,1> */
