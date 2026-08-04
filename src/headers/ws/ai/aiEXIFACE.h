#pragma once
// ws-engine ai subsystem: aiEXIFACE — the AI "external interface" facade the rest of the
// engine calls into (nav-editor queries, team validation, nav-system lookup, weapon-user
// registration). The class itself carries no state (DB `types` size = 1, an empty class);
// every method delegates to module-global singletons (aiPlanner / aiPlanIFace / gsTEAMS).
//
// Cross-subsystem pointee types are OTHER-SUBSYSTEM BOUNDARIES for this wave and are
// forward-declared only (next frontier): navSYS/navMANAGER/nedNAV_EDITOR (nav),
// aiPLANNER/aiPLANNER_IFACE/aiBRAIN (ai planner core), propWPN_AI_USABLE (prop),
// gsTEAMS (gs teams), aiTEAMS_SYS (ai teams).

#include "../ds/dsSTRID.h"
#include "../ds/dsTSTRING.h"

struct navSYS;              // nav subsystem — a navigation graph/system                 boundary (fwd)
struct navMANAGER;          // nav subsystem — owns/looks up navSYS instances            boundary (fwd)
struct nedNAV_EDITOR;       // nav subsystem — interactive nav-mesh editor               boundary (fwd)
struct aiPLANNER;           // ai planner core — global AI planner                       boundary (fwd)
struct aiPLANNER_IFACE;     // ai planner core — planner virtual interface (IsEnemy...)  boundary (fwd)
struct aiBRAIN;             // ai — per-actor brain (gsIs<aiBRAIN> RTTI test)            boundary (fwd)
struct propWPN_AI_USABLE;   // prop subsystem — AI-usable weapon property                boundary (fwd)
struct entENTITY;           // ent subsystem — root game entity                          boundary (fwd)

struct aiEXIFACE {
    // 0x82D313F8 — ID of the waypoint currently selected in the nav editor (delegates to
    // aiPlanner->GetNavEditor()->GetSelectedWP_ID()).
    int GetNavEdSelectedWPID() const;

    // 0x82D31440 — true when `wpID` is the nav editor's currently selected waypoint.
    bool IsNavEdSelectedWP(short wpID) const;

    // 0x82D314D8 — true when (`nsID`,`edgeID`) is the nav editor's selected edge.
    bool IsNavEdSelectedEdge(short nsID, int edgeID) const;

    // 0x82D31590 — if `team` is neither a predefined team nor registered with gsTEAMS, log a
    // warning and reset it to the neutral team id.
    void EnsureTeamIsValid(dsSTRID &team);

    // 0x82D31638 — true when `team` is hostile to `teamTo` (planner IsEnemy when aiPlanIFace is
    // present, else the OPP/NEUTRAL fallback rules).
    bool IsTeamOppositeTo(const dsSTRID &team, const dsSTRID &teamTo);

    // 0x82D316F0 — true when the team named by `teamID` is predefined or registered.
    bool IsTeamValid(const dsTSTRING<char> &teamID);

    // 0x82D31968 — find a nav system by name via the planner's nav manager (nullptr if none).
    navSYS *FindNavSys(const char *nameSys);

    // 0x82D319D0 — get a nav system by id (nullptr if the planner has no nav manager).
    navSYS *GetNavSys(short nsID);

    // 0x82D31A38 — const overload of GetNavSys.
    const navSYS *GetNavSys(short nsID) const;

    // 0x82D31AB0 — register `user` as a user of AI-usable weapon `usable`.
    void RegisterWeaponUser(propWPN_AI_USABLE *usable, entENTITY *user);

    // 0x82D31B40 — inverse of RegisterWeaponUser.
    void UnRegisterWeaponUser(propWPN_AI_USABLE *usable, entENTITY *user);

    // 0x82D31D18 — true when `shooter` is an aiBRAIN that reports "skip destruction" (virtual).
    bool IsSkipDestr(entENTITY *shooter);

    // 0x82D31DA0 (?GetDbgWatcheeName@aiEXIFACE@@QAA?AV?$dsTSTRING@D@@XZ) — display name of the
    // brain the debugger currently watches, or "none" when nothing is watched. sret.
    // REVERSED: src/ws/ai/aiEXIFACE__GetDbgWatcheeName.cpp.
    dsTSTRING<char> GetDbgWatcheeName();
};

struct aiBRAIN_IFACE; // aiBRAIN_IFACE.h — ai brain public interface

// Global accessor: the brain the debugger is currently watching (null when none). boundary.
aiBRAIN_IFACE *GetBrainDebugWatchee();

// Module-global singletons the facade delegates to (defined in the ai planner TU) — boundary.
extern aiPLANNER *aiPlanner;
extern aiPLANNER_IFACE *aiPlanIFace;
// Interned id of the predefined "opponent/OPP" team — boundary global (gs teams).
extern dsSTRID TEAM_OPP;
