#pragma once
#include "../ds/dsFLAGS.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsSTRID.h" // RADAR::TOUCH::teamID
#include "../m3d/m3dV.h"
#include "../m3d/m3dMATR.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsSHARED_PTR.h"
#include "../ds/Deleter.h"
#include "aiVIS_SUCCESS.h"
#include "aiVIS_QUERY.h"
#include "aiVIS_RESULTS.h"
#include "aiSENSES.h"
#include "../m3d/m3dSPL_LINEAR1D.h"
#include "aiDANGER.h"
#include "STRIKE_MODIFIERS.h"
// ws-engine ai08: a brain's per-frame runtime status block (aiBRAIN::st.pointee).
// DB-verified layout (types_members aiSTATUS) — size 0x6B8. Only the members reached by this batch
// (gen, body, wpn, cs, die, smtDeathMaxDist, dns) are typed; the intervening sub-blocks
// (peekerSelf/coordGroup/aim/goal/mind/est/lod/enemy/perc/radar/danger) are kept as byte-accurate
// opaque placeholders so the offsets stay exact (their bodies are the next frontier).

struct aiSTATUS_vtbl;
struct animINST; // anim subsystem — animated instance (fwd, boundary)
struct aiWATCHER_BRAIN; // aiWATCHER_BRAIN.h — owning watcher back-pointer (fwd, boundary)

// Flag-enum tags (bit namespaces). Opaque here — the enumerators live in the enum-value tables and
// are the next frontier; only the packed integer value is read.
enum AI_GEST  : int;
enum AI_BOST  : int;
enum AI_WEST  : int;
enum AI_MIST  : int; // ai08 — mind-state flag enum
enum AI_DIEST : int;
enum AI_ENEST : int; // ai08 — enemy-estimate flag enum (bit 0x10 = enemy visible now)

struct aiSTATUS {
    // aiSTATUS::CS — coordinate-system sub-block. DB-verified layout (types_members aiSTATUS::CS)
    // — size 208 (0xD0).
    struct CS {
        m3dV          posBody;          // 0x00
        m3dV          dirBody;          // 0x0C
        m3dV          dirUpBody;        // 0x18
        m3dV          dirRightBody;     // 0x24
        m3dMATR       mBodyLCSbyUp;     // 0x30 body local-coordinate-system matrix (DB: m3dMATR)
        m3dV          posFace;          // 0x70
        m3dV          dirFace;          // 0x7C
        m3dV          posInst;          // 0x88
        m3dV          dirInst;          // 0x94
        m3dV          dirUpInst;        // 0xA0
        m3dV          dirRightInst;     // 0xAC
        m3dV          dirUBody;         // 0xB8
        m3dV          dirUBodyRight;    // 0xC4

        // ai08 setters used by aiBRAIN::UpdateBodyCS — bodies external to this batch. boundary.
        void Set(const m3dV &pos, const m3dV &dir, const m3dV &dirUp);
        void Inst(const m3dV &pos, const m3dV &dir, const m3dV &dirUp);
        void UpperBody(const m3dV &dirUp);

        // 0x831789AC-region call — seed the combat sub-block's initial values from an instance.
        // Body external to this batch. boundary.
        void FillInitial(animINST *pInst);
    };

    // aiSTATUS::DNS — dynamic-navigation-sampling params. DB-verified: nPoints@0, distMin@4,
    // distMax@8 — size 12.
    struct DNS {
        int   nPoints; // 0x00
        float distMin; // 0x04
        float distMax; // 0x08
    };

    // aiSTATUS::PERC — perception snapshot sub-block (size 0x74). Only the touched fields are typed.
    struct PERC {
        bool          isClipUpAngle;      // 0x00
        bool          isClipDownAngle;    // 0x01
        aiSENSES         senses;               // 0x04 sight/hearing radii + cone angles
        m3dSPL_LINEAR1D  viewPercRaisingSpeed; // 0x20 distance->view-percent raising-speed curve
        int              viewPrecent;          // 0x34 current reported view-percent (0..100)
        float         distToReticle;      // 0x38 distance brain->reticle
        m3dV          dirToReticle;       // 0x3C reticle direction (world)
        float            grAvoidRadius;   // 0x48 grenade-avoidance radius
        dsVECTOR<int, 8> grenadeMPIDs;    // 0x4C ids of grenades queued for avoidance (size 0x14)
        bool          needToReduceVision;  // 0x60 request: shrink vision cone next tick
        bool          needToEnhanceVision; // 0x61
        unsigned char _pad62[0x64 - 0x62];
        float         delayOnAlertMin;    // 0x64
        float         delayOnAlertMax;    // 0x68
        bool          shouldUseManualViewPercent; // 0x6C use viewPercForAlert instead of the est bit
        unsigned char _pad6D[0x70 - 0x6D];
        float         viewPercForAlert;   // 0x70 manual view-percent alert threshold
    };

    // aiSTATUS::AIM — aiming sub-block (size 340 / 0x154). Only contourShooting.isEnabled is typed;
    // everything else stays byte-accurate opaque (next frontier).
    struct AIM {
        // aiSTATUS::AIM::CONTOUR_SHOOTING (size 0x20) — DB: isEnabled@0x1C.
        struct CONTOUR_SHOOTING {
            unsigned char _pad00[0x1C]; // 0x00 dist(m4dV)+shift(m3dV) (opaque)
            bool          isEnabled;    // 0x1C
            unsigned char _pad1D[0x20 - 0x1D];
        };
        // aiSTATUS::AIM::ADV_SNIPER_MODIFIER (DB size 0xD4). Embeds STRIKE_MODIFIERS::ADV_SNIPER_PARAMS
        // at 0x00 (params.isEnabled@0x00); only the fields reached by aiWEAPON strike-phase logic are
        // typed, the rest stays byte-accurate opaque (next frontier).
        struct ADV_SNIPER_MODIFIER {
            // STRIKE_MODIFIERS::ADV_SNIPER_PARAMS (size 0x74): isEnabled@0x00.
            struct PARAMS {
                bool          isEnabled;    // 0x00
                unsigned char _pad01[0x74 - 0x01];
            };
            PARAMS        params;           // 0x00 (0x74)
            unsigned char _pad74[0x78 - 0x74]; // 0x74 state (enum)
            bool          isFinished;       // 0x78
            unsigned char _pad79[0xD4 - 0x79]; // 0x79 needInit/init-shift/timers etc (opaque)
        };
        m3dV             posBasicTarget;      // 0x00
        m3dV             pointForProjectile;  // 0x0C aim point used for projectile prediction
        dsTSTRING<char>  nameGiven;           // 0x18 name of the scripted aim tracker ("" when none)
        STRIKE_MODIFIERS::AIM_POINT_TYPE typeAimPos; // 0x1C
        float            shiftAimH;           // 0x20
        float            timerToFreezeAim;    // 0x24 countdown after which aim may re-freeze
        unsigned char    _pad28[0x60 - 0x28]; // 0x28 throwerParams/sniperParams (opaque)
        CONTOUR_SHOOTING contourShooting;     // 0x60 (0x20)
        ADV_SNIPER_MODIFIER advSniper;        // 0x80 (0xD4, ends 0x154)
    };

    // aiSTATUS::LOD — level-of-detail sub-block (size 8). DB-verified.
    struct LOD {
        int   value;            // 0x00
        float timeKeepRequests; // 0x04

        // ?Reset@LOD@aiSTATUS@@QAAXXZ @ 0x83175820 — clear the LOD sub-block to its default state.
        void Reset();
    };

    // aiSTATUS::RADAR — pathing/radar sub-block (size 60 / 0x3C). DB-verified layout
    // (types_members aiSTATUS::RADAR).
    struct RADAR {
        // aiSTATUS::RADAR::TOUCH (DB size 20): point@0, teamID@0xC, isFilled@0x10.
        struct TOUCH {
            m3dV    point;    // 0x00 last radar touch point
            dsSTRID teamID;   // 0x0C team of what was touched
            bool    isFilled; // 0x10
        };
        float        timeStuck;   // 0x00
        float        timeWaiting; // 0x04
        float        paramWait;   // 0x08
        unsigned int blockerUID;  // 0x0C uid of the blocker (-1 = none)
        m3dV         trIn;        // 0x10
        m3dV         trOut;       // 0x1C
        TOUCH        touch;       // 0x28 (0x14, ends 0x3C)
    };

    // aiSTATUS::ENEMY — enemy-tracking sub-block (size 0x384). DB-verified layout (headers_ref
    // aiSTATUS::ENEMY). Only the fields reached by aiDEBUG::RenderEnemyGroup are typed; the
    // intervening vis-query/result blocks stay byte-accurate opaque (next frontier).
    struct ENEMY {
        CS            cs;              // 0x000 enemy coordinate-system snapshot
        m3dV          posNav;         // 0x0D0 enemy nav-space position
        aiVIS_QUERY   visQuery;       // 0x0DC pending visibility probe (size 0x64)
        aiVIS_RESULTS visResult;      // 0x140 latest visibility probe result (size 0xE0)
        aiVIS_SUCCESS visLastSuccess; // 0x220 last successful visibility sample
        m3dV          posVisLast;     // 0x308 last position seen at
        unsigned char _pad314[0x344 - 0x314]; // 0x314 adjVisCheck/posAimingLCS/posBlankShootLCS/to (opaque)
        float         dist;           // 0x344 straight-line distance brain->enemy
        unsigned char _pad348[0x354 - 0x348]; // 0x348 distXZ/heightDiff/heightPrefVis (opaque)
        float         heightMinFlesh; // 0x354 min flesh height above feet
        float         height;         // 0x358 full body height
        unsigned char _pad35C[0x384 - 0x35C]; // 0x35C angles/UID/quarterVolume/possible (opaque)

        // Clear this enemy-tracking sub-block back to "no enemy". Used by aiMIND::ZeroEnemy.
        // boundary — body external to this batch.
        void Reset();
    };

    // GOAL — movement-goal snapshot. DB-verified layout (types_members aiSTATUS::GOAL), size 20.
    struct GOAL {
        float timePosReached; // 0x00 accumulated time the goal position has been reached
        float heightDiff;     // 0x04 vertical delta body->goal
        float distXZ;         // 0x08 horizontal (XZ) distance body->goal
        float angle;          // 0x0C
        float angleAbs;       // 0x10
    };

    aiSTATUS_vtbl       *__vftable;        // 0x000
    dsFLAGS<AI_GEST, int> gen;             // 0x004 general-state bitflags
    aiWATCHER_BRAIN      *peekerSelf;      // 0x008 owning watcher back-pointer
    unsigned char        _pad00C[0x14 - 0x0C]; // 0x00C coordGroup/aimCorrectionType (opaque)
    aiWATCHER_BRAIN      *dbgDuplicatePeekerSelf; // 0x014 debug duplicate of peekerSelf
    dsFLAGS<AI_BOST, int> body;            // 0x018 body-state bitflags
    dsFLAGS<AI_WEST, int> wpn;             // 0x01C weapon-state bitflags
    CS                    cs;              // 0x020 combat sub-block (208)
    GOAL                  goal;            // 0x0F0 aiSTATUS::GOAL (20)
    AIM                   aim;             // 0x104 aiming sub-block (size 0x154, ends 0x258)
    dsFLAGS<AI_MIST, int> mind;            // 0x258 mind-state bitflags
    dsFLAGS<AI_ENEST, int> est;            // 0x25C enemy-estimate bitflags
    LOD                   lod;             // 0x260 level-of-detail sub-block (size 8, ends 0x268)
    dsFLAGS<AI_DIEST, int> die;            // 0x268 death-state bitflags
    float                 smtDeathMaxDist; // 0x26C
    ENEMY                 enemy;           // 0x270 enemy-tracking sub-block (size 0x384, ends 0x5F4)
    PERC                  perc;            // 0x5F4 perception snapshot (size 0x74)
    RADAR                 radar;           // 0x668 pathing/radar sub-block (size 0x3C, ends 0x6A4)
    DNS                   dns;             // 0x6A4 dynamic-nav-sampling params
    dsSHARED_PTR<aiDANGER, 0, Deleter<aiDANGER> > danger; // 0x6B0 active danger being reacted to

    // 0x83228500 (?SetPar@aiSTATUS@@QAA_NABV?$dsTSTRING@D@@M@Z) — set a named float/bool tunable on
    // this status block. REVERSED: aiSTATUS__SetPar.cpp.
    bool SetPar(const dsTSTRING<char> &key, float val);

    // The coordinated group cached on this status block (the coordGroup sub-block, opaque above),
    // or null when the unit is ungrouped. Dispatched by aiCOORDINATOR::GetGroup. boundary.
    struct aiCOORD_GROUP *GetCoordGroup();
};
