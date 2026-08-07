#pragma once
#include "../anim/animINST.h"
#include "../anim/animTPL.h"
#include "../cdt/cdtCOLL_ANIM_SCN.h"
#include "../ds/dsFIXED_BIT_ARRAY.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsTSTRING.h"
#include "../ap/apSTATE_T.h"
#include "../ps/psSECTION.h"
#include "../m3d/m3dBOX.h"
#include "../os/osLOCK.h"
#include "../ds/dsGUID.h"
#include "scnPS_CACHE.h"
// ws-engine scn: the scene object — owns the pool of animINST instances (doubly-linked live list
// `pFirstInst`/`animINST::prev`/`next`, plus a singly-linked free list `pFirstFree` through the same
// `next` field) backed by the fixed `instList` array. DB-verified layout (types_members scnSCENE) —
// size 10352 (0x2870).

struct cdtREFINE;      // collision-query refinement params (pointer only)             boundary
struct cdtINFO;        // collision-query result record (pointer only)                 boundary
struct cdtCOLL_SCN;    // scene collision manager (pointer only)                      boundary
struct cdtCOLL_LIGHT;  // scene light-collision manager (pointer only)                boundary
struct selSEL;         // selection-set object (pointer only)                        boundary
struct camCAMERA;      // active camera (pointer only)                               boundary
struct scnINST_CREATE_DATA; // pending-creation record, dsVECTOR element (pointer-level only) boundary
struct lgtLIGHT;       // scene light record, dsVECTOR element (pointer-level only)   boundary

typedef struct scnSCENE {
    // Nested lookup-entry type used only as a dsVECTOR element (pointer-level only); boundary.
    struct scnTPL_LOOKUP_ENTRY_TYPE; // boundary

    osLOCK             *cdtSceneLock;      // 0x0000
    cdtCOLL_SCN         *pColl;            // 0x0004
    cdtCOLL_ANIM_SCN    *pCollAnim;        // 0x0008 collision-animation manager
    cdtCOLL_LIGHT       *pCollLight;       // 0x000C
    m3dBOX               bbox;             // 0x0010
    m3dBOX               bboxExtInst;      // 0x0028
    dsFIXED_BIT_ARRAY<2500> isUsedInstState; // 0x0040 one bit per instList slot (Yes==allocated/attached)
    char                 name[64];         // 0x2750
    apSTATE_T<unsigned long> stateScn;     // 0x2790
    objOBJ               *pObj;            // 0x2794 scene root model object
    animTPL              *tplList;         // 0x2798 fixed template pool
    animINST             *instList;        // 0x279C fixed instance pool (indexed by animINST::id)
    dsVECTOR<scnINST_CREATE_DATA, 8> cdListScene; // 0x27A0
    animINST             *pFirstInst;      // 0x27B4 head of the live (attached) doubly-linked list
    animINST             *pFirstFree;      // 0x27B8 head of the free singly-linked list (via ->next)
    dsVECTOR<lgtLIGHT, 8> lightList;        // 0x27BC
    int                   lastBigLightNmb; // 0x27D0
    dsTSTRING<char>      *navDataPS;       // 0x27D4
    dsVECTOR<unsigned char, 8> *navDataBin;// 0x27D8
    int                   nObjSel;         // 0x27DC
    selSEL               *objSelList;      // 0x27E0
    psSECTION             ps;              // 0x27E4
    camCAMERA            *pCam;            // 0x27E8
    animTPL               *pTplDummy;      // 0x27EC fallback template used when CreateInst gets a null pTpl
    scnPS_CACHE            psCache;        // 0x27F0 (20B — dsVECTOR<scnPS_CACHE::REC,8>)
    int                   nObj;            // 0x2804
    objOBJ               **ppObjList;      // 0x2808
    dsVECTOR<objOBJ *, 16> objListVisible;      // 0x280C
    dsVECTOR<m3dBOX, 16>   objListVisibleBBOX;  // 0x2820
    dsVECTOR<scnTPL_LOOKUP_ENTRY_TYPE, 8> tplLookup; // 0x2834
    dsGUID                 guid;           // 0x2848 (16B — unsigned int d[4])
    int                   countShootRays;  // 0x2858
    dsVECTOR<int, 8>       instIndJustCreated; // 0x285C pending just-created instance ids (sorted, dedup)

    // ---- template store (boundary; DB-confirmed scnSCENE members) ----

    // 0x8253C6A8 (?FindTplName@scnSCENE@@QAAPAVanimTPL@@PBD@Z) — locate an already-resident
    // template by (extension-stripped) name; returns pTplDummy on miss. boundary.
    animTPL *FindTplName(const char *name);
    // 0x8253EAA0 (?AllocTpl@scnSCENE@@QAAPAVanimTPL@@XZ) — allocate an empty template slot
    // (null on exhaustion). boundary.
    animTPL *AllocTpl();
    // 0x8253EBC8 (?DestroyTpl@scnSCENE@@QAAXPAVanimTPL@@@Z) — release a template slot
    // allocated via AllocTpl. boundary.
    void     DestroyTpl(animTPL *pTpl);
    // 0x827430B8 (?IsBelongPosSceneBox@scnSCENE@@QAAHPAUm3dV@@@Z) — nonzero when world position
    // `pos` lies inside the scene's bounding box. boundary.
    int IsBelongPosSceneBox(m3dV *pos);

    // ---- reversed in the ws_scn_0001 batch ----

    // 0x8253C718 — the instance following `pInst` in the live list, or the list head when
    // `pInst` is null.
    animINST *FindNextInst(animINST *pInst);

    // 0x8253CFA8 — case-insensitive scan of the live list for the first instance whose name
    // matches; treats a null per-instance name as the shared empty string. Empty/null `name`
    // never matches (returns null immediately).
    animINST *FindInstName(const char *name);

    // 0x8253CE60 (private, AAA mangle) — unlink `pInst` from the live doubly-linked list, clear
    // its bit in `isUsedInstState`, and null out its prev/next/pScn links. Handles the
    // sole-remaining-node case (pInst is head with no next) separately from the general case.
    void _DetachInst(animINST *pInst);

    // 0x8253CF18 — release collision/collision-anim state, detach from the live list, reset the
    // instance to its cleared state, then push it onto the free list (using ->next).
    void DestroyInst(animINST *pInst);

    // 0x82540BD8 — insert `id` into the sorted `instIndJustCreated` set (duplicate ids ignored).
    // NOTE: the decompiler fabricated 13 extra trailing int parameters (a3..a14) on this function;
    // disasm confirms only r3 (this) and r4 (id) are ever read — the real prototype takes a single
    // int. The phantom params also leaked into the AllocInst/CreateInst decompiles below (verified
    // and stripped there too).
    void AddJustCreatedInstances(int id);

    // 0x82540C90 — pop the head of the free list (or, if empty, dump the live list to
    // "scn_inst.csv" and strong-assert-crash) and initialise it as a fresh instance for `pTpl`:
    // identity transform, unit scale, sign stamped, no physics/no LOD-loaded. When `isAttach` is
    // set, also splices it onto the head of the live list and marks its `isUsedInstState` bit.
    animINST *AllocInst(animTPL *pTpl, int isAttach);

    // 0x82540DD0 — allocate an instance from `pTpl` (or `pTplDummy` when null) via AllocInst
    // (unattached), duplicate the template's model-object tree if any (destroying the instance on
    // failure), apply names/create-data/section overrides, merge template state flags, then splice
    // it onto the live list and mark it attached.
    animINST *CreateInst(animTPL *pTpl, animCREATE_DATA *pCD);

    // 0x82744BB4-region — core closest-point-on-scene-collision query. boundary within this
    // batch (cdt-owned collision math); declared so GetClosestPoint_EXT can delegate to it.
    int GetClosestPoint_INT(m3dV *vTo, float radius, cdtREFINE *pRefine, cdtINFO *pInfo);

    // 0x82744B58 — thin wrapper: zero the caller's extended cdtINFO output block (9 dwords / 36
    // bytes) when supplied, then delegate to GetClosestPoint_INT. NOTE: the DB's demangled
    // prototype lists only (m3dV*, float, cdtREFINE*, cdtINFO*); the function body plus every call
    // site show a trailing cdtINFO* output register (r7) that this wrapper clears — modeled here
    // as `pInfoExt`.
    int GetClosestPoint_EXT(m3dV *vTo, float radius, cdtREFINE *pRefine, cdtINFO *pInfo,
                            cdtINFO *pInfoExt);
} scnSCENE;
