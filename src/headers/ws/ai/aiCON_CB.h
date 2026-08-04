#pragma once
#include "../con_/CONSOLE_CALLBACK.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsFLAGS.h"
#include "../ds/WEAK_PTR.h"
// ws-engine ai08: the AI debug-console page callback. Publicly derives from
// con::CONSOLE_CALLBACK and drives the "AI" console page — pushing watchee/AI-count/SSL-var
// lists to the remote debug console and handling its requests. DB-verified layout
// (types_members aiCON_CB) — size 0x58; base con::CONSOLE_CALLBACK occupies 0x00..0x10.

struct aiBRAIN;   // ai08 — the AI brain object (full definition aiBRAIN.h)  boundary (ptr/weak)
struct fioFILE;   // fio  — message stream                                  boundary (ptr)

// AI info-list line flags — dsFLAGS<AI_INFO_LINE,int> bitmask selecting which per-watchee
// info lines are pushed. DB-verified (types_enum_values AI_INFO_LINE).
enum AI_INFO_LINE {
    AI_ILINE_ENE_NAME      = 0x00001,
    AI_ILINE_ENE_VIS       = 0x00002,
    AI_ILINE_LOF           = 0x00004,
    AI_ILINE_NS            = 0x00008,
    AI_ILINE_COORDSQUAD    = 0x00010,
    AI_ILINE_OBSA          = 0x00020,
    AI_ILINE_TEAM          = 0x00040,
    AI_ILINE_ISA           = 0x00080,
    AI_ILINE_LOD           = 0x00100,
    AI_ILINE_RETICLE       = 0x00200,
    AI_ILINE_AIM           = 0x00400,
    AI_ILINE_GRENADES      = 0x00800,
    AI_ILINE_WEAPONS       = 0x01000,
    AI_ILINE_MANIP         = 0x02000,
    AI_ILINE_TASKS         = 0x04000,
    AI_ILINE_TASK_FLAGS    = 0x08000,
    AI_ILINE_MOVEMENTSPEED = 0x10000,
    AI_ILINE_PLATT         = 0x20000,
    AI_ILINE_PROPS         = 0x40000,
    AI_ILINE_LASTSTATE     = 0x40001,
};

// The kind of extra info-list requested for the selected watchee. DB-verified
// (types_enum_values aiINFO_LIST_TYPE).
enum aiINFO_LIST_TYPE {
    AI_ILIST_UNDEF = -1,
    AI_ILIST_BHV   = 0,
    AI_ILIST_TASK  = 1,
    AI_ILIST_LAST  = 2,
};

namespace ai { namespace con_cb {
// The current SSL-var list mode for the AI console page. DB-verified
// (types_enum_values ai::con_cb::INFO_LIST_TYPE).
enum INFO_LIST_TYPE {
    ilMAX_VALUE = -1,
    ilUNDEF     = -1,
    ilOFF       = 0,
    ilSslBrain  = 1,
    ilSslSquad  = 2,
    ilStrikes   = 3,
};
} } // namespace ai::con_cb

typedef struct aiCON_CB : con::CONSOLE_CALLBACK {
    // base con::CONSOLE_CALLBACK: __vftable@0x00, pageName@0x04, isVisible@0x08,
    // pageVersionValid@0x0C
    dsTSTRING<char>                    msgCtrlName;      // 0x10 console message control name
    bool                               isConnected;      // 0x14
    float                              timer;            // 0x18 change-throttle countdown
    ai::con_cb::INFO_LIST_TYPE         infoListType;     // 0x1C active SSL-var list mode
    ai::con_cb::INFO_LIST_TYPE         nextInfoListType; // 0x20 pending mode (applied next flush)
    bool                               haveSslVarsReq;   // 0x24
    bool                               wSelectFastPeek;  // 0x25
    dsFLAGS<AI_INFO_LINE, int>         infoSelectorMain; // 0x28 main info-line selector mask
    aiINFO_LIST_TYPE                   infoSelectorExtra;// 0x2C extra info-list selector
    ds::WEAK_PTR<aiBRAIN>              pCurrentWatchee;  // 0x30 currently-inspected brain
    int                                idxLiveWat;       // 0x34
    int                                idxDeadWat;       // 0x38
    int                                coLiveUnits;      // 0x3C last-sent live unit count
    int                                coDeadUnits;      // 0x40 last-sent dead unit count
    dsTSTRING<char>                    aiListSSLVars;    // 0x44 last-sent SSL-var list text
    dsTSTRING<char>                    aiListMain;       // 0x48
    dsTSTRING<char>                    aiListExtra;      // 0x4C
    dsTSTRING<char>                    aiTaskSys;        // 0x50
    dsTSTRING<char>                    aiBhvStack;       // 0x54

    // --- reversed in this batch ---
    // Recompute the live/dead unit + watcher-index tallies and, when changed (or forced),
    // push a "#N: L live/ D dead" summary (plus a LEAKMEM warning line) to the console.
    bool SendAICountToConsole(bool force);
    // Build the SSL-var list text for the selected watchee's brain into `list`.
    void GetAISSLVarsBrain(dsTSTRING<char> &list);
    // Squad SSL-var list — currently a fixed "not supported" placeholder.
    void GetAISSLVarsSquad(dsTSTRING<char> &list);
    // Dispatch a console request for the AI page (list/line selectors, SSL-vars mode, watchee
    // fast-peek/select-next/clear/kill). Overrides con::CONSOLE_CALLBACK::OnMsg.  virtual
    int  OnMsg(const dsTSTRING<char> &ctrl, const dsTSTRING<char> &msg, fioFILE &data,
               fioFILE *response);
    // Produce the SSL-var list text for the current mode/watchee into `list`.
    bool GetAISSLVars(dsTSTRING<char> &list);
    // Recompute the SSL-var list and, when changed (or forced), push it to the console.
    bool SendListSSLVars(bool force);
    // Push every AI console sub-list unconditionally (used on (re)connect / page enable).
    bool SendAll();
    // Per-frame change flush: push any changed sub-lists, and every ~1s push the AI count.
    bool SendChanges(float dt);
    // Toggle the AI console page on/off remotely, reset selectors, and SendAll().
    void EnableConPageAI(bool enable);

    // Compare `fresh` against the last-sent `placed`; when they differ (or `force`), copy `fresh`
    // into `placed` and push it to the console as message `msg`. REVERSED: aiCON_CB__StoreSend.cpp.
    void StoreSend(bool force, const dsTSTRING<char> &fresh, dsTSTRING<char> &placed, const char *msg);

    // --- boundary (bodies are the next frontier / defined in sibling aiCON_CB TUs) ---
    bool IsConnected();                                  // virtual (con page connected)  boundary
    void GetAICount(int *pLive, int *pDeadUnits, int *pIdxLive, int *pIdxDead, int *pExtra); // boundary
    void AddLineStrikes(dsTSTRING<char> &list);          // boundary
    bool SendMainList(bool force);                       // boundary
    bool SendExtraList(bool force);                      // boundary
    bool SendTaskSys(bool force);                        // boundary
    bool SendBhvStack(bool force);                       // boundary
    bool SendWatcheeToConsole(bool force);               // boundary
    void SendSelectorMain(int mask);                     // boundary
    void SendSelectorExtra(int mask);                    // boundary
    void WSelectNextLive();                              // boundary
    void WSelectNextDead();                              // boundary
} aiCON_CB;
