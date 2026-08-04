#pragma once
#include "../ds/dsTSTRING.h"
#include "../ds/dsVECTOR.h"
#include "navBLOCK_OBB.h"
// ws-engine nav subsystem — nav-mesh manager (boundary subset). Canonical home for the navMANAGER
// interface previously duplicated across aiEXIFACE_boundaries.h and aiPLANNER_boundary.h. Only the
// methods the ai08 planner / exiface batches actually call are declared; the full navMANAGER class
// (WP tables, HO-jump batches, dom-sys nav) is the next re-source frontier. Not an authoritative
// layout — no data members are asserted.

struct navSYS;   // nav subsystem — a single nav-system            boundary (fwd)
struct aiBRAIN;  // ai08 — per-unit AI brain                       boundary (fwd)
struct navWP;    // nav — a nav waypoint                           boundary (fwd)
struct aiDOM_NAV;// nav domain (ai08)                              boundary (fwd)
struct animINST; // anim instance                                  boundary (fwd)

struct navMANAGER {
    // ?GetWP@navMANAGER@@... — waypoint by id (null when absent).  boundary.
    navWP *GetWP(short wpID);
    // Autolink waypoint `wpID` to its neighbours.  boundary.
    void   AutolinkWP(short wpID);
    // Queue a high-obstacle jump batch build for waypoint `wp`/`wpID`.  boundary.
    void   AddHOJumpBatch(navWP *wp, short wpID);

    // ?FindWP@navMANAGER@@QAAPAVnavWP@@PBD@Z @ 0x8318DED8 — waypoint by name (null when absent).
    navWP *FindWP(const char *name);
    // ?FindWP_ID@navMANAGER@@QAAFPBD@Z — waypoint id (short) by name.  boundary.
    short  FindWP_ID(const char *name);

    // ?FindNavSys@navMANAGER@@QAAPAVnavSYS@@PBD@Z
    navSYS *FindNavSys(const char *name);
    // ?RelinkWPToNS@navMANAGER@@QAA_NPAVnavSYS@@FPAVnavWP@@AAV?$dsTSTRING@D@@@Z — move waypoint
    // `wpID`/`wp` into nav-system `ns`; false with a reason in `errHint` on failure.  boundary.
    bool    RelinkWPToNS(navSYS *ns, short wpID, navWP *wp, dsTSTRING<char> &errHint);
    // ?GetNavSys@navMANAGER@@QAAPAVnavSYS@@F@Z
    navSYS *GetNavSys(short nsID);
    // ?GetNavSysCount@navMANAGER@@QBAHXZ
    int     GetNavSysCount() const;
    // ?CanSetupInitial@navMANAGER@@QAA_NPAVaiBRAIN@@PAV?$dsTSTRING@D@@@Z
    bool    CanSetupInitial(aiBRAIN *pBrain, dsTSTRING<char> *rejectReason);
    // ?DbgReasonWaitSetupInitial@navMANAGER@@QAA_NAAV?$dsTSTRING@D@@@Z (reference param modelled as
    // a pointer to match the a8_planner.cpp call sites).
    bool    DbgReasonWaitSetupInitial(dsTSTRING<char> *reason);

    // ?BlockByVolList@navMANAGER@@QAAXAAV?$dsVECTOR@UnavBLOCK_OBB@navGPH@@$07@@KG@Z
    // Mark every waypoint/edge intersecting one of `vols` as blocked; `flagsBlockedWP` selects the
    // WP-block bit set and `flagsDisabledEdge` the edge-disable bit set.  boundary.
    void BlockByVolList(dsVECTOR<navGPH::navBLOCK_OBB, 8> &vols,
                        unsigned int flagsBlockedWP, unsigned short flagsDisabledEdge);
    // ?UnblockByVolList@navMANAGER@@QAAXAAV?$dsVECTOR@UnavBLOCK_OBB@navGPH@@$07@@KG@Z — inverse.
    void UnblockByVolList(dsVECTOR<navGPH::navBLOCK_OBB, 8> &vols,
                          unsigned int flagsBlockedWP, unsigned short flagsDisabledEdge);
    // ?AutolinkInstWPs@navMANAGER@@QAAXPAVanimINST@@@Z — (re)link the waypoints owned by `pInst`.
    void AutolinkInstWPs(animINST *pInst);
    // ?DisconnectInstWPs@navMANAGER@@QAAXPAVanimINST@@@Z — sever the links of `pInst`'s waypoints.
    void DisconnectInstWPs(animINST *pInst);
    // ?GetNavDomain@navMANAGER@@QAAPAVaiDOM_NAV@@ABV?$dsTSTRING@D@@@Z — nav-domain by name (null if absent).
    aiDOM_NAV *GetNavDomain(const dsTSTRING<char> &name);
    // 0x8318EF88 (?ProcessFrame@navMANAGER@@QAAXXZ) — per-frame nav update. boundary.
    void ProcessFrame();
    // 0x83193028 (?InitLevel@navMANAGER@@QAAXXZ) — per-level init. boundary.
    void InitLevel();
    // 0x83193090 (?TermLevel@navMANAGER@@QAAXXZ) — per-level teardown. boundary.
    void TermLevel();
    // 0x8321D7A0 (?AddSceneTplToQueue@navMANAGER@@QAAXPAVentENTITY@@@Z) — queue a scene template
    // entity for nav processing. boundary.
    void AddSceneTplToQueue(entENTITY *pEnt);
};
