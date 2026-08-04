#pragma once
#include "../ds/dsTSTRING.h"
#include "../con_/MSG_PROTOCOL.h"
// Boundary globals & shims used by the aiCON_CB console-page methods. These live in other TUs
// (the AI console module's file-statics and the ap debug console); declared extern here since
// this re-source splits the original conAI TU across sibling .cpp files.

struct aiBRAIN;   // ai08 — AI brain (full definition aiBRAIN.h)  boundary
struct aiPLANNER; // ai08 — AI planner                            boundary

// The ap debug console message-protocol backend (byte_82... in the original).
extern con::MSG_PROTOCOL *apConsole;

// The globally-selected AI debug brain (con "watchee kill" target). DB: ?_pBrainDebugWatchee@@3PAVaiBRAIN@@A
extern aiBRAIN *_pBrainDebugWatchee;

// The AI planner singleton; null before a map is loaded.
extern aiPLANNER *aiPlanner;

// Single NUL byte at 0x8200155A used as an empty C-string seed for dsTSTRING::UnsafeInit.
extern const char g_emptyString_8200155A[];

// --- console message-name / request-name string constants (con page wire vocabulary) ---
extern const dsTSTRING<char> sn_WatcheeCount_1;            // "WatcheeCount"
extern const dsTSTRING<char> sn_SSLVars_1;                 // "SSLVars"
extern const dsTSTRING<char> sn_EnablePage_1;              // "EnablePage"
extern const dsTSTRING<char> pgReq_ListSelector_1;         // list-selector request
extern const dsTSTRING<char> pgReq_LineSelector_1;         // line-selector request
extern const dsTSTRING<char> pgReq_SSLVars_1;              // SSL-vars mode request
extern const dsTSTRING<char> pgReq_WatcheeFastPeek_1;      // fast-peek toggle request
extern const dsTSTRING<char> pgReq_WatcheeSelectNext_1;    // select-next-watchee request
extern const dsTSTRING<char> pgReq_WatcheeClear_1;         // clear-watchee request
extern const dsTSTRING<char> pgReq_WatcheeKill_1;          // kill-watchee request
extern const dsTSTRING<char> strNoWatchee_1;               // "<<no watchee>>" placeholder text
extern const dsTSTRING<char> sn_WatcheeInfo_1;             // "WatcheeInfo" message name

// --- boundary member-call shims (bodies external; member functions on boundary types) ---
extern int  aiPLANNER_GetLeakBalanceBehaviors(aiPLANNER *planner); // aiPLANNER::GetLeakBalanceBehaviors
extern void aiBRAIN_Die(aiBRAIN *brain, const dsTSTRING<char> &reason); // aiBRAIN::Die

// dsSPrintf — format into a fresh dsTSTRING<char> (returned by value). boundary.
extern dsTSTRING<char> dsSPrintf(const char *fmt, ...);
