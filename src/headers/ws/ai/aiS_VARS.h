#pragma once
#include "../m3d/m3dV.h"
#include "../ds/WEAK_PTR.h"
#include "aiS_TIE.h"
// ws-engine ai08: per-strike cached working variables for a selected weapon strike. Holds the
// resolved interface/controller weak pointers, the muzzle tip pos/dir, the parabolic-trajectory
// prediction, and the family of validity/cool-down timers the weapon module ages each frame.
// DB-verified layout (types_members aiS_VARS): size 92.

struct wpnWEAPON;         // wpn — a single equipped weapon (fwd)
struct propWPN_AI_USABLE; // prop — AI-usable weapon controller (fwd)
struct aiBRAIN;           // ai08 — brain (fwd)

struct aiS_VARS {
    // aiS_VARS::CUT_OFF_CRITERIA — why a strike's trajectory got cut off (debug). boundary enum.
    enum CUT_OFF_CRITERIA { CUT_OFF_NONE = 0 };

    aiS_TIE                        tie;                // 0x00 the tie this vars block belongs to
    bool                           isCacheValid;       // 0x14
    ds::WEAK_PTR<wpnWEAPON>         iface;             // 0x18 resolved weapon interface
    ds::WEAK_PTR<propWPN_AI_USABLE> ctrl;             // 0x1C resolved controller
    m3dV                           posTip;             // 0x20 muzzle tip position
    m3dV                           dirTip;             // 0x2C muzzle firing direction
    float                          parabolicPrediction;// 0x38 parabolic aim height addition
    float                          startSpeed;         // 0x3C computed launch speed
    bool                           isEnabledByScript;  // 0x40
    bool                           isEnabledByFilter;  // 0x41
    bool                           isEnabledByCtrl;    // 0x42
    bool                           isTrajectoryOK;     // 0x43
    const aiBRAIN                 *trajFriendBlocker;  // 0x44
    float                          timerTrajValid;     // 0x48
    float                          timerTrajFresh;     // 0x4C
    float                          timerSparse;        // 0x50
    float                          timerCoolDown;      // 0x54
    CUT_OFF_CRITERIA               dbgReason;          // 0x58

    // ?UpdateTimers@aiS_VARS@@QAAXM_N@Z — age this strike's timers by `dt`; `isActive` selects
    // whether the cool-down/valid timers count down. boundary — body external to this batch.
    void UpdateTimers(float dt, bool isActive);
};
