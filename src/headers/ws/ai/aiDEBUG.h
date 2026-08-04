#pragma once
#include "../m3d/m3dV.h"
#include "../m3d/m3dMATR.h"
#include "../m3d/m3dOBB.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsCONST_ARRAY.h"
#include "../ds/dsSTRID.h"
#include "../ap/apSTATE_T.h"
#include "CONST_ARRAY_m3dV.h"
// ws-engine ai08 (a8_debug.cpp): per-brain visual-debug scratch/state block. Lives inside aiBRAIN
// (aiBRAIN.dbg). DB-verified layout (headers_ref aiDEBUG) — size 0x152C.

struct aiBRAIN;         // owning brain (fwd)
struct navASSOC_INFO;   // nav subsystem — waypoint-association debug record (aiDEBUG_boundaries.h)
struct aiDBGSHAPE_VERT; // ai08 — debug mesh vertex (aiDEBUG_boundaries.h)
struct dsVECTOR_PARAM_LIST;                 // ds — owning parameter list (dsVECTOR_PARAM_LIST.h)
struct navWP_ID_WEIGHT;                     // nav — waypoint id + weight pair (aiPLANNER graph)
namespace ds_data { template<class T> struct REF_TYPE; } // ds — ref-counted typed handle (REF_TYPE.h)

// aimSent flags — debug marker of which aim visualisation was last emitted. Opaque int enum
// (enumerators are the next frontier); only the packed value is stored.
enum aiDBGAIM_SENT : int { };

struct aiDEBUG {
    // Simple coloured/sized position marker.
    struct POS_VIEW {           // size 0x14
        m3dV         pos;       // 0x00
        unsigned int color;     // 0x0C
        float        size;      // 0x10
    };

    // Grenade/projectile trajectory visualisation (parabolic + linear + insertion arcs).
    struct TRAJECTORY_VISUALIZATION { // size 0xF4
        dsCONST_ARRAY<m3dV, 10> parabPoints;      // 0x00
        bool                    hasParabIntersec; // 0x7C
        m3dV                    intersecParab;    // 0x80
        m3dV                    linearFrom;       // 0x8C
        m3dV                    linearMiddle;     // 0x98
        m3dV                    linearTo;         // 0xA4
        bool                    hasLinearIntersec;// 0xB0
        m3dV                    linearObst;       // 0xB4
        m3dV                    insFrom;          // 0xC0
        m3dV                    insMiddle;        // 0xCC
        m3dV                    insTo;            // 0xD8
        bool                    hasInsIntersec;   // 0xE4
        m3dV                    insObst;          // 0xE8
    };

    // Flying-object (thrown grenade/obstacle) OBB visualisation.
    struct FLY_OBJ {            // size 0x78
        m3dOBB Obb;            // 0x00
        m3dOBB trajObb;        // 0x3C
    };

    // Grenade-avoidance line visualisation.
    struct GRENADE_AVOID_LINE { // size 0x54
        m3dV posStart;          // 0x00
        m3dV posEndReal;        // 0x0C
        m3dV posEndHor;         // 0x18
        m3dV posClosestToBrain; // 0x24
        m3dV posBrain;          // 0x30
        m3dV CDTtest_start;     // 0x3C
        m3dV CDTtest_success;   // 0x48
    };

    // A from/to pair of world positions.
    struct m3dVPair {           // size 0x18
        m3dV posFrom;           // 0x00
        m3dV posTo;             // 0x0C
    };

    // Vision-cone probe visualisation.
    struct VISION {             // size 0x94
        dsVECTOR<m3dVPair, 8> lines;        // 0x00
        bool                  wasCollision; // 0x14
        CONST_ARRAY_m3dV<10>  posCollisions;// 0x18
    };

    // Find-enemy probe visualisation.
    struct FIND_ENEMY {         // size 0x14
        dsVECTOR<m3dVPair, 8> lines; // 0x00
    };

    // Last-used FSM speed / event debug strings.
    struct FSMSTATUS {          // size 0x8
        dsTSTRING<char> lastUsedSpeed;         // 0x00
        dsTSTRING<char> lastUsedSpeedFsmEvent; // 0x04
    };

    // Dynamic-nav-sampling point visualisation.
    struct DNS {                // size 0x28
        dsVECTOR<m3dV, 8> points;   // 0x00
        dsVECTOR<m3dV, 8> pntCheck; // 0x14
    };

    aiBRAIN                   *pBrain;                        // 0x0000
    const char                *msgAimHead;                   // 0x0004
    dsSTRID                    msgAimTail;                    // 0x0008
    aiDBGAIM_SENT              aimSent;                       // 0x000C
    const char                *minilogEnterLeaveHO;          // 0x0010
    apSTATE_T<unsigned long>   state;                        // 0x0014
    m3dV                       vCheckFrom[10];               // 0x0018
    m3dV                       vCheckTo[10];                 // 0x0090
    m3dV                       visionCylSlice[10];           // 0x0108
    m3dV                       vDirFace;                     // 0x0180
    m3dV                       vOrgFace;                     // 0x018C
    m3dV                       strafeFrom[2];                // 0x0198
    m3dV                       strafeTo[2];                  // 0x01B0
    m3dV                       otherTempLinesFrom[100];      // 0x01C8
    m3dV                       otherTempLinesTo[100];        // 0x0678
    int                        otherTempLinesCount;          // 0x0B28
    m3dV                       otherTempPos[100];            // 0x0B2C
    int                        otherTempPosCount;            // 0x0FDC
    unsigned int               Color;                        // 0x0FE0
    m3dV                       vCylFrom;                     // 0x0FE4
    m3dV                       vCylTo;                       // 0x0FF0
    float                      radCyl;                       // 0x0FFC
    m3dV                       vRadarFrom;                   // 0x1000
    m3dV                       vRadarTo;                     // 0x100C
    m3dV                       vRadarHit;                    // 0x1018
    dsVECTOR<m3dMATR, 8>       csList;                       // 0x1024
    dsVECTOR<POS_VIEW, 8>      posList;                      // 0x1038
    dsTSTRING<char>            lineOnBhvEnd;                 // 0x104C
    unsigned int               possibleEnemiesUID[32];       // 0x1050
    m3dV                       posBasicAiming;               // 0x10D0
    m3dV                       posPrevBasicAiming;           // 0x10DC
    m3dV                       pointForProjectile;           // 0x10E8
    m3dV                       vUpRight;                     // 0x10F4
    m3dV                       vDownRight;                   // 0x1100
    m3dV                       vDownLeft;                    // 0x110C
    m3dV                       vUpLeft;                      // 0x1118
    bool                       havePenaltyEnterHO;           // 0x1124
    m3dV                       posCenterPenaltyEnterHO;      // 0x1128
    m3dV                       posThrowObjStart;             // 0x1134
    m3dV                       dirThrowObj;                  // 0x1140
    m3dV                       posThrowObjClosest;           // 0x114C
    dsTSTRING<char>            reasonRejectWaitInitialSetup; // 0x1158
    TRAJECTORY_VISUALIZATION   traj;                         // 0x115C
    m3dV                       posEnemyVisCheck;             // 0x1250
    m3dV                       posEye;                       // 0x125C
    m3dV                       dirSight;                     // 0x1268
    m3dV                       posPassedWpnTgt;              // 0x1274
    FLY_OBJ                    flyObj;                       // 0x1280
    GRENADE_AVOID_LINE         grenadeAvoidLine;             // 0x12F8
    dsVECTOR<m3dV, 8>          CDTtest_ends;                 // 0x134C
    dsVECTOR<m3dV, 8>          CDTtest_fails;                // 0x1360
    dsVECTOR<m3dVPair, 8>      grenades;                     // 0x1374
    VISION                     vision;                       // 0x1388
    FIND_ENEMY                 find_enemy;                   // 0x141C
    FSMSTATUS                  fsmStatus;                    // 0x1430
    DNS                        dns;                          // 0x1438
    short                      dbgGroupWpSetFiltered[100];   // 0x1460
    int                        dbgGroupWpSetFilteredCount;   // 0x1528

    // ---- reversed in this batch (a8_debug.cpp) ----

    // ?IsOffAI@aiDEBUG@@SAHXZ — static: true when the global off-AI debug switch is set.
    static int IsOffAI();

    // 0x83193D78 (?IsRenderIngameText@aiDEBUG@@SA_NXZ) — static: true when in-game AI debug text is
    // being rendered this frame. Used by aiBRAIN::UpdateFlags.  boundary.
    static bool IsRenderIngameText();

    // 0x83194968 (?IsGodMode@aiDEBUG@@SAHXZ) — static: true when the AI god-mode cheat is active
    // (AI units ignore incoming damage).  boundary.
    static int IsGodMode();

    // ?HasMovingWatchee@aiDEBUG@@SA_NXZ — static: true when a debug-tracked watchee is currently
    // moving (used to keep per-frame AI ticks running while AI is otherwise switched off). boundary.
    static bool HasMovingWatchee();

    // 0x83194A28 — draw the enter-hideout penalty centre sphere when the debug var is set.
    void RenderHOGroup();

    // 0x83194C98 — draw the full enemy-tracking visualisation (thrown-object, coordinate systems,
    // distance, height bars, last-visible, reticle, quarter-volume) gated by per-feature debug vars.
    void RenderEnemyGroup();

    // 0x831956E0 — push a timed debug dot primitive at `pos`.
    void PushTimedDot(m3dV *pos, unsigned int color, const char *nameFmt);

    // 0x83195A80 — record/clear the "OnBhvEnd(<name>)" debug line.
    void MarkOnBhvEnd(const char *name);

    // 0x83195B10 — push a timed debug cylinder (axis vFrom->vTo, radius) as a set of line primitives.
    void PushTimedCylinder(m3dV *vFrom, m3dV *vTo, float radius, const char *nameFmt);

    // 0x83196928 — per-frame refresh of cached face org/dir + ingame-text render toggle.
    void ProcessFrame();

    // 0x831969F8 — emit a "~Rules~" log line for `oper` tagged with the source file/line.
    void LogRule(const char *file, int line, const char *oper);

    // 0x83194AA0 / 0x83194AE8 / 0x83194B30 / 0x83194B98 — format "<oper> : <value>" for a debug
    // console line (int / float / vector / string overloads). static.
    static dsTSTRING<char> Par2Str(const char *oper, int value);
    static dsTSTRING<char> Par2Str(const char *oper, float value);
    static dsTSTRING<char> Par2Str(const char *oper, m3dV &value);
    static dsTSTRING<char> Par2Str(const char *oper, const char *value);

    // ---- reversed in THIS batch (a8_debug.cpp) ----

    // ?RenderDebugInfoWPs@aiDEBUG@@SAXPBFHK@Z @0x83193D88 — static: draw a debug cross at every
    // waypoint id in `debugInfo[0..count)`, plus its HO plane-normal line. REVERSED.
    static void RenderDebugInfoWPs(const short *debugInfo, int count, unsigned long color);

    // ?RenderDebugInfoAssoc@aiDEBUG@@SAXPBUnavASSOC_INFO@@HKK@Z @0x83193E90 — static: draw one line
    // per association record, tinted by its visibility flag. REVERSED.
    static void RenderDebugInfoAssoc(const navASSOC_INFO *assoc, int count,
                                     unsigned long colorEnabled, unsigned long colorDisabled);

    // ?InterpolateColor@aiDEBUG@@IAAKKKHH@Z @0x83193EF8 — protected: lerp `colorFrom`->`colorTo`
    // across `maxColors` steps at step `idx` (packed 0x00RRGGBB). REVERSED.
    unsigned long InterpolateColor(unsigned int colorFrom, unsigned int colorTo,
                                   int maxColors, int idx);

    // ?GrabPathPts@aiDEBUG@@QBAXAAV?$dsVECTOR@Um3dV@@$07@@_N@Z @0x831944C0 — const: copy the brain's
    // current path points into `pathPts` (optimized set when `getOptimized`). REVERSED.
    void GrabPathPts(dsVECTOR<m3dV, 8> &pathPts, bool getOptimized) const;

    // ?GrabHOPathPts@aiDEBUG@@QBAXAAV?$dsVECTOR@Um3dV@@$07@@AAM1@Z @0x831945E0 — const: copy the
    // brain's high-obstacle path points into `pathPts`, reporting start/end. REVERSED. (mangled
    // params are float& — reproduced as float* per the call-site, ABI-identical.)
    void GrabHOPathPts(dsVECTOR<m3dV, 8> &pathPts, float *posStart, float *posEnd) const;

    // ?RenderDebugInfoMesh@...@0x83194690 — static: submit an indexed debug triangle mesh at alpha
    // `alpha`. REVERSED. (4 params — the decompiler's spurious 5th arg dropped, disasm-verified.)
    static void RenderDebugInfoMesh(const dsVECTOR<aiDBGSHAPE_VERT, 8> &vBuf,
                                    dsVECTOR<unsigned short, 8> &iBuf, float alpha, bool zTest);

    // ?IsBackOffProcessFrame@aiDEBUG@@QBAHXZ @0x831949C0 — const: true when the brain's per-frame
    // processing should be skipped (no object, locked, or off-AI debug hold). REVERSED.
    int IsBackOffProcessFrame() const;

    // ---- reversed in the ai render wave (a8_debug.cpp) ----

    // 0x83196BF8 — static: append the state param-list `p`'s "<name> = <value>" lines to `line`.
    static void AppendStateParamList(const ds_data::REF_TYPE<dsVECTOR_PARAM_LIST> &p,
                                     dsTSTRING<char> &line);

    // 0x83197C20 — static: copy the first min(from.nElem, capacity) waypoint ids of `from` into
    // `debugInfo`, writing the copied count into `*count`.
    static void FillDebugRenderInfoWPs(const dsVECTOR<navWP_ID_WEIGHT, 8> &from, short *debugInfo,
                                       int debugInfoCapacity, int *count);

    // 0x83197CF0 — protected: draw the enabled coordinate-system debug gizmos (inst/nav/root/list).
    void RenderCoordSysGroup();
    // 0x83198178 — protected: draw the weapon/aiming/trajectory debug visualisation group.
    void RenderWeaponGroup();
    // 0x83199558 — protected: draw the fan/behaviour-line/temp-line/temp-pos/IK debug group.
    void RenderOtherTempGroup();
    // 0x83199FC8 — protected: draw the three hearing-radius circles around the body.
    void RenderHearing();

    // 0x8319A0F0 — protected: build the flat sight-shape polygon (`shape`) from the eye position and
    // forward/right directions using the brain's sight cone angles/ranges.
    void GetSightShape(const m3dV &startPos, const m3dV &dirForward, const m3dV &dirRight,
                       dsVECTOR<m3dV, 8> &shape);

    // ---- reversed in THIS batch (a8_debug.cpp) ----

    // 0x8319AEF0 — protected: draw the sight-line + vision-cone/simple-cone visualisation.
    void RenderSight();
    // 0x8319B220 — protected: draw the whole perception debug group (sight, vision, hearing,
    // find-enemy, vision-cyl-slice, line-of-fire, possible-enemies, radar) gated by con-vars.
    void RenderPercGroup();
    // 0x8319BC60 — draw the full per-brain AI debug data (all render groups) unless the brain
    // is fully spawned.
    void RenderAIData();

    // ---- render-group helpers dispatched by RenderAIData (bodies external to this batch) ----
    void RenderCDTGroup();      // boundary — collision-detection debug gizmos
    void RenderGoalGroup();     // boundary — goal/path debug gizmos
    // Build the multi-ring vision cone polylines from the eye pos and forward/right dirs. boundary.
    void CreateVisionCone(const m3dV *startPos, const m3dV *dirForward, const m3dV *dirRight,
                          dsVECTOR<dsVECTOR<m3dV, 8>, 8> *out);

    // Global AI-debug toggle: when set, mind frames are forced on regardless of LOD. Used by
    // aiMIND::UpdateSparser.  boundary — body external to this batch.
    static bool IsOffMind();
};
