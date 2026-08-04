#pragma once
#include "../msg/msgADDR.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsPAIR.h"
#include "../ds/dsTSTRING.h"
#include "../ap/apSTATE_T.h"
#include "../ds/dsSTRID.h"
#include "../ds/dsSMART_PTR.h"
#include "../ds/WEAK_PTR.h"
#include "../m3d/m3dMATR.h"
#include "../m3d/m2dV.h"
#include "../obj/objOBJ.h"
// ws-engine gs: the in-game cinematic ("anitec") director/tick system — owns the active
// cinematic's actor bindings, camera-shake state, playback timeline, and ssl callbacks.
// DB-verified layout (types_members gsANITEC_SYS):
//   base msgADDR@0 (32), base mp::MSG_IFACE@32 (4), shaker@36 (172), shaker_info@208 (24),
//   pCurActor@232, pObjCam@236, pObjTgt@240, angleOld@244, aspectOld@248, cineLength@252,
//   actors@256 (20), sslFuncOnStart@276, sslFuncOnEnd@280, sslFuncOnCancel@284,
//   ownedActors@288 (20), clipFrames@308 (20), stateCin@328, trackTime@332 (8),
//   anitecQueue@340, anitecSkipFrames@344, m_socket@348, isPaused@349.
// The two zero-named leading members (offset 0 size 32 / offset 32 size 4) are anonymous
// base-class slots per the DB — modeled as public multiple inheritance (msgADDR, mp::MSG_IFACE).

struct gsANITEC_ACTOR; // boundary — per-actor cinematic runtime record (boundary)
struct entENTITY;      // boundary — ws entity base class (boundary)

namespace mp {
    struct MSG_IFACE_vtbl; // boundary
    // boundary — ws multiplayer message-interface mixin (base class of gsANITEC_SYS).
    // DB-verified layout (types_members mp::MSG_IFACE): __vftable@0 — size 4.
    struct MSG_IFACE {
        MSG_IFACE_vtbl *__vftable; // 0x00
    };

    // boundary — mp::SOCKET_STATIC. DB size 1 with no member rows (empty marker/mixin
    // type); spelled as an empty struct to match.
    struct SOCKET_STATIC {
    };
}

namespace plrCAM_SHAKE {
    struct SHAKE;      // boundary — one active shake record (vector element)
    struct SHAKE_INFO; // boundary — one shake descriptor (vector element)

    // Player camera-shake runtime state. DB-verified layout (types_members
    // plrCAM_SHAKE::SHAKER) — size 172.
    struct SHAKER {
        dsVECTOR<SHAKE, 8>       shakes;               // 0x00
        float                    curTime;              // 0x14
        dsSTRID                  camMode;              // 0x18
        dsSTRID                  camModeShake;         // 0x1C
        m3dMATR                  shakeAffectingPlr;    // 0x20
        m3dMATR                  shakeNotAffectingPlr; // 0x60
        m2dV                     shakeRecoilDelta;     // 0xA0
        ds::WEAK_PTR<entENTITY>  pOwner;               // 0xA8
    };
    // Player camera-shake descriptor set. DB-verified layout (types_members
    // plrCAM_SHAKE::SHAKER_INFO) — size 24.
    struct SHAKER_INFO {
        dsVECTOR<SHAKE_INFO, 8>  shakeInfos; // 0x00
        bool                     updated;    // 0x14
        unsigned char            _pad15[3];  // 0x15 db-verified padding
    };
}

struct sslSTANDALONE_FUNCTION_IMPL; // boundary — ssl standalone-function body

// Ref-counted smart pointer to a bound ssl standalone function. DB-verified: a single
// anonymous base dsSMART_PTR<sslSTANDALONE_FUNCTION_IMPL,_dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> >
// at offset 0 — size 4 (one owning pointer); ref-count/PLC machinery is a boundary.
struct sslSTANDALONE_FUNCTION
    : dsSMART_PTR<sslSTANDALONE_FUNCTION_IMPL, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > {
};

struct gsANITEC_SYS : public msgADDR, public mp::MSG_IFACE {
    // gsANITEC_SYS::ACTOR — per-actor cinematic binding record (element type of `actors`
    // below). Only ever stored by pointer inside dsVECTOR here, so left incomplete —
    // boundary (boundary) (DB shows it has nested ANIM/SCRIPT/SOUND_TRACK
    // sub-vectors of its own).
    struct ACTOR;

    // gsANITEC_SYS::TRACK_TIME — cinematic track-time state.
    // DB-verified layout (types_members gsANITEC_SYS::TRACK_TIME): key@0, ofs@4 — size 8.
    struct TRACK_TIME {
        int   key; // 0x00
        float ofs; // 0x04
    };

    plrCAM_SHAKE::SHAKER       shaker;            // 0x024 (36)  camera-shake runtime state
    plrCAM_SHAKE::SHAKER_INFO  shaker_info;       // 0x0D0 (208) camera-shake descriptor
    gsANITEC_ACTOR             *pCurActor;        // 0x0E8 (232) actor driving the active cinematic (null when idle)
    objOBJ                     *pObjCam;          // 0x0EC (236) cinematic camera object
    objOBJ                     *pObjTgt;          // 0x0F0 (240) cinematic look-at/target object
    float                       angleOld;         // 0x0F4 (244) saved pre-cinematic camera angle
    float                       aspectOld;        // 0x0F8 (248) saved pre-cinematic aspect ratio
    int                         cineLength;        // 0x0FC (252) total cinematic length
    dsVECTOR<ACTOR, 8>          actors;            // 0x100 (256) per-actor cinematic bindings
    sslSTANDALONE_FUNCTION      sslFuncOnStart;    // 0x114 (276) ssl callback fired on cinematic start
    sslSTANDALONE_FUNCTION      sslFuncOnEnd;      // 0x118 (280) ssl callback fired on cinematic end
    sslSTANDALONE_FUNCTION      sslFuncOnCancel;   // 0x11C (284) ssl callback fired on cinematic cancel/abort
    dsVECTOR<entENTITY *, 8>    ownedActors;       // 0x120 (288) entities spawned/owned by the cinematic
    dsVECTOR<dsPAIR<int, int>, 8> clipFrames;      // 0x134 (308) per-clip frame-range table
    apSTATE_T<unsigned long>    stateCin;          // 0x148 (328) cinematic playback state bits
    TRACK_TIME                  trackTime;         // 0x14C (332) cinematic track-time state
    dsTSTRING<char>              anitecQueue;      // 0x154 (340) queued cinematic name
    int                          anitecSkipFrames; // 0x158 (344) frames to skip on (re)start
    mp::SOCKET_STATIC            m_socket;         // 0x15C (348) MP replication socket state
    bool                         isPaused;         // 0x15D (349) cinematic paused flag

    // 0x825B5B20 — abort the active cinematic: if a cinematic actor is bound, tears it
    // down via OnCineEnd(true) (the "was aborted" path).
    void AbortCinematic();

    // 0x825C1D28 — protected virtual; the C++ mangle is `MAAX_N@Z` (protected, virtual,
    // returns void, one bool arg). Vtable slot 0x2C (44) — the second virtual gsANITEC_SYS
    // adds beyond msgADDR's own 10-entry vtable (which ends at 0x28). Tears down the active
    // cinematic; `wasAborted` distinguishes a natural finish from an abort/cancel. Called
    // through the vtable at the AbortCinematic call site — boundary (boundary).
    virtual void OnCineEnd(bool wasAborted); // 0x825C1D28 — boundary
};
