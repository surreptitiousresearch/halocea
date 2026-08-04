#pragma once
// Boundary interface subsets for aiEXIFACE's delegates. These declare ONLY the member/static
// functions aiEXIFACE actually calls — they are NOT full class layouts (the real definitions
// live in the nav / ai-planner / gs-teams subsystems and are the next re-source frontier).
// No data members are asserted here; do not treat these as authoritative sizes.

#include "../ds/dsSTRID.h"
#include "../nav/navMANAGER.h"
#include "../gs/gsTEAMS.h"
#include "aiTEAMS_SYS.h"
#include "aiPLANNER.h"

struct navSYS;

// nav subsystem — interactive nav-mesh editor (boundary subset).
struct nedNAV_EDITOR {
    int  GetSelectedWP_ID();
    bool IsSelectedWP_2(short wpID);
    bool IsSelectedEdge_2(short nsID, int edgeID);
    // Add waypoint id `wpID` to the editor's "unused" set. Used by aiPLANNER::NoticeNavEdUnusedWP.
    void AddToUnused(short wpID);
};

// navMANAGER is defined once in nav/navMANAGER.h (included above).
// aiPLANNER is defined once in aiPLANNER.h (included above); GetNavEditor/GetNavManager live there.
// gsTEAMS is defined once in gs/gsTEAMS.h (included above); Instance() returns a reference.

// ai planner core — planner virtual interface (boundary subset).
struct aiPLANNER_IFACE {
    bool IsEnemy(const dsSTRID &team, const dsSTRID &teamTo); // virtual in the real class
};

// aiTEAMS_SYS is defined once in aiTEAMS_SYS.h (included above). Its IsPredefinedTeam static takes
// a const dsSTRID* in the canonical header (matching aiTEAMS_SYS__IsPredefinedTeam.cpp); the two
// EXIFACE consumers pass &team accordingly.

struct aiBRAIN;   // ai — per-actor brain (RTTI test)                       boundary (fwd)
struct msgADDR;   // ws msg system — RTTI-addressable base                  boundary (fwd)

// ws RTTI helper: nonzero when `addr` dynamically is-a T. Real symbol for the aiBRAIN
// instantiation is ?$gsIs@VaiBRAIN@@ ... @@YAHPBVmsgADDR@@@Z — boundary.
template <class T> int gsIs(const msgADDR *addr);

// Strong-assert plumbing (boundary) — mirrors the cseries STRONG_ASSERT macro. The canonical
// STRONG_ASSERT_DUMMY (with its static Crash overloads) lives in dbg/STRONG_ASSERT_DUMMY.h.
#include "../dbg/STRONG_ASSERT_DUMMY.h"
extern int IGNORE_STRONG_ASSERT;               // _IGNORE_STRONG_ASSERT__3HA
extern unsigned char STRONG_ASSERT_INFO_STUB;  // empty_string (assert "info" byte)
