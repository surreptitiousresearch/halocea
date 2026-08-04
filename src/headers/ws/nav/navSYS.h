#pragma once
#include "../ds/dsTSTRING.h"
#include "../ds/dsVECTOR.h"
#include "../m3d/m3dV.h"
#include "../m3d/m3dBOX.h"
#include "navWP.h"

// ws-engine nav subsystem — a single nav-system (waypoint graph). Only nameSys@0 and the methods
// the planner calls are modeled; the remainder of navSYS is a boundary (next frontier). navSYS is
// used by pointer only here, so leaving the body incomplete-after-nameSys is safe.

struct navQUERY;
struct navPATH;
struct navWP_SET;   // nav — a named waypoint set / front  boundary (ptr)

// DB size = 196 (0xC4). Full DB layout (types_members navSYS, re-checked 2026-08-04). The embedded
// navGPH sub-object at +0x08 stays a size-exact byte region: the DB `navGPH` class is modeled as a
// *namespace* corpus-wide (navGPH_boundary.h — navGPH::navBLOCK_OBB), so a `struct navGPH` here
// would clash in the aiPHYS_SMR TUs.
struct navSYS {
    dsTSTRING<char>            nameSys;      // 0x00 nav-system name
    short                      nsID;         // 0x04 nav-system id (DB-verified)
    unsigned char              _pad06[2];    // 0x06
    unsigned char              gph[80];      // 0x08 DB: navGPH (namespace/class modeling clash — see above)
    dsVECTOR<navWP_SET *, 8>   wpSetCache;   // 0x58 (88)
    m3dBOX                     aabb;         // 0x6C (108)
    bool                       isSmallSize;  // 0x84 (132) small-unit nav-system flag (DB-verified)
    unsigned char              _pad85[3];    // 0x85
    m3dV                       dirUp;        // 0x88 (136)
    bool                       isDomainBased; // 0x94 (148)
    bool                       isStatic;     // 0x95 (149) scripted static-marking flag (disasm-verified)
    bool                       isUnblockable; // 0x96 (150)
    unsigned char              _pad97;       // 0x97
    int                        counterBrains;   // 0x98 (152)
    int                        counterPlayers;  // 0x9C (156)
    bool                       isMonitored;     // 0xA0 (160)
    unsigned char              _padA1[3];       // 0xA1
    float                      timerMotion;     // 0xA4 (164)
    float                      timerQGMotion;   // 0xA8 (168)
    float                      timeLastValidation; // 0xAC (172)
    float                      distCloseClip;   // 0xB0 (176)
    int                        _idxDbgFrom;     // 0xB4 (180)
    int                        _idxDbgTo;       // 0xB8 (184)
    int                        isConnectivityBroken;  // 0xBC (188)
    int                        isConnectivityUpdated; // 0xC0 (192)

    // ?Term@navSYS@@QAAXXZ — tear the nav-system down (drop its waypoint graph). boundary.
    void   Term();
    // ?AddRef@navSYS@@QAAXPAVaiBRAIN@@@Z / ?Release@navSYS@@QAAXPAVaiBRAIN@@@Z — add/drop a brain's
    // reference to this nav system (ref-count bookkeeping). boundary — body external to this batch.
    void   AddRef(struct aiBRAIN *pBrain);
    void   Release(struct aiBRAIN *pBrain);
    // Defined elsewhere (nav).  boundary
    int    GetNWP() const;
    navWP *GetWP(short idx);
    bool   IsAABBBelongPoint(const m3dV *pos);
    // ?IsWpSetReady@navSYS@@QBA_NABV?$dsTSTRING@D@@@Z — true when the named WP-set/front is loaded.
    bool   IsWpSetReady(const dsTSTRING<char> &frontName) const;
    bool   IsAcceptDirUp(const m3dV *up);
    bool   BuildPath(navQUERY *query, navPATH *outPath);
    // 0x8322D2E8 (?LinkScriptedOp@navSYS@@QAAXPBD0_N1ABV?$dsTSTRING@D@@@Z) — create a scripted
    // navigation link between the two named waypoints. boundary.
    void   LinkScriptedOp(const char *wpA, const char *wpB, bool a3, bool a4,
                          const dsTSTRING<char> &smtObjName);
    // ?GetWpSet@navSYS@@QAAPAVnavWP_SET@@ABV?$dsTSTRING@D@@@Z @ 0x8322DC48 — named WP-set/domain
    // (null when absent).  boundary.
    navWP_SET *GetWpSet(const dsTSTRING<char> &name);
    // ?PenalizeWPSet@navSYS@@QAAXPAVnavWP_SET@@H@Z @ 0x832476D8 — bump the traversal penalty of every
    // waypoint in `wpSet` by `level` (forwards into the embedded navGPH at +0x08).  boundary.
    void   PenalizeWPSet(navWP_SET *wpSet, int level);
};
