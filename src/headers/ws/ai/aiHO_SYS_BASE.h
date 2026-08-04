#pragma once
#include "../ds/dsVECTOR.h"
#include "../m3d/m3dV.h"           // HO_EDGE_PATH_QUERY::prefDir is m3dV by value
// ws-engine ai08: base of the "hint object" (HO) query subsystem a brain uses to pick cover /
// vantage waypoints. DB-verified layout (types_members aiHO_SYS_BASE): __vftable@0 — the rest is
// the NEXT FRONTIER. This batch reaches two virtual query stubs.

struct aiHO_SYS_BASE_vtbl;
struct hoEDGE;
struct mdlITRC_HO;
struct navWP;

struct aiHO_SYS_BASE {
    aiHO_SYS_BASE_vtbl *__vftable; // 0x00

    // DB-verified (types_members aiHO_SYS_BASE::HO_EDGE_PATH_QUERY), size 0x2C — a request to
    // build an edge path from `eFrom`@`posFrom` to `eTo`@`posTo` across the HO edge graph.
    struct HO_EDGE_PATH_QUERY {
        mdlITRC_HO *itrc;      // 0x00
        bool        is3D;      // 0x04
        hoEDGE     *eFrom;     // 0x08
        float       posFrom;   // 0x0C
        hoEDGE     *eTo;       // 0x10
        float       posTo;     // 0x14
        bool        havePrefDir; // 0x18
        m3dV        prefDir;   // 0x1C
        bool        allowMove; // 0x28
        bool        allowFlip; // 0x29
        bool        allowOuter;// 0x2A
    };

    // How the desired HO position is derived from the waypoint (DB enum aiHO_SYS_BASE::HO_SHIFT).
    enum HO_SHIFT {
        HO_SHIFT_DISABLED    = 0, // project the reference vector onto the HO-plane direction
        HO_SHIFT_TO_CORNER   = 1, // shift straight to the reference (corner) vector
        HO_SHIFT_TO_BODYPOS  = 2, // shift to the body-entry position (records last entry on the brain)
        HO_SHIFT_TO_ENTERPOS = 3, // shift to the enter position
    };

    // DB enum aiHO_SYS_BASE::aiSPEC_HO_TYPE.
    enum aiSPEC_HO_TYPE {
        AI_SPEC_HO_ADJACENT = 0,
        AI_SPEC_HO_FORWARD  = 1,
        AI_SPEC_HO_BOTTOM   = 2,
    };

    // 0x832A58C0 (?ReconsiderHO@aiHO_SYS_BASE@@UAAFFF@Z) — virtual. Base default: ping the planner's
    // waypoint accessor for `howpid`/`wpidIgnore` (side effect only) and echo `howpid` back.
    // REVERSED: aiHO_SYS_BASE__ReconsiderHO.cpp.
    virtual short ReconsiderHO(short howpid, short wpidIgnore);

    // 0x83179C20 (?FindHOClosestVis@aiHO_SYS_BASE@@UAAFFAAV?$dsVECTOR@F$07@@FAAF@Z) — virtual.
    // This build's body is a stub: it copies the first candidate of `fromArr` into `wpidFrom`
    // and returns `wpidTo`.
    virtual short FindHOClosestVis(short wpidParent, dsVECTOR<short, 8> &fromArr,
                                   short wpidTo, short &wpidFrom);

    // 0x8317A690 (?FindHOClosestTo@aiHO_SYS_BASE@@UAA_NFFFAAV?$dsVECTOR@F$07@@@Z) — virtual.
    // This build's body is a stub: it resets `closestArr` and pushes `wpidFrom` as the sole
    // result; returns true.
    virtual bool FindHOClosestTo(short wpidParent, short wpidFrom, short wpidTo,
                                 dsVECTOR<short, 8> &closestArr);

    // 0x832A6570 (?GetHOEdge@aiHO_SYS_BASE@@SAPAVhoEDGE@@AAVnavWP@@@Z) — static: enter the hideout
    // described by waypoint `wp` and return the edge its controller landed on (or nullptr). REVERSED.
    static hoEDGE *GetHOEdge(navWP &wp);

    // 0x832A6AC0 (?BuildHOEdgePath@aiHO_SYS_BASE@@UAA_NABUHO_EDGE_PATH_QUERY@1@AAV?$dsVECTOR@...@@@Z) —
    // virtual: build the edge path for `query` into `ePath`; true on success. REVERSED.
    virtual bool BuildHOEdgePath(const HO_EDGE_PATH_QUERY &query, dsVECTOR<hoEDGE *, 8> &ePath);

    // 0x832A6F48 (?BuildHOEdgePathRec@aiHO_SYS_BASE@@MAA_NABUHO_EDGE_PATH_QUERY@1@AAV?$dsVECTOR@...@@MAAM@Z)
    // — protected virtual: recursive DFS builder; accumulates `len`, keeps best in `bestLen`. REVERSED.
    virtual bool BuildHOEdgePathRec(const HO_EDGE_PATH_QUERY &query, dsVECTOR<hoEDGE *, 8> &ePath,
                                    float len, float &bestLen);

    // 0x83176168 (?ProcessFrame@aiHO_SYS_BASE@@UAAXM@Z) — virtual: per-frame hideout-system update.
    // Used by aiBODY_IFACE::ProcessFrame.  boundary.
    virtual void ProcessFrame(float dt);

    // vtbl helpers whose bodies are the next frontier (declared so this class's TUs close). boundary.
    bool IsHO3DMode(mdlITRC_HO *itrc);                              // boundary
    bool IsValidEdge(const HO_EDGE_PATH_QUERY &query);             // boundary
};
