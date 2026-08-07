#pragma once
#include <stddef.h> // wchar_t
#include "../msg/msgADDR.h"
#include "../ap/apSTATE_T.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsEVENT_HANDLER.h"
#include "../m3d/m3dV.h"
#include "../m3d/m2dV.h"
// ws-engine inp: the Kinect input subsystem message-actor. DB-verified layout (types_members
// gsINP_KINECT, 844 bytes; row 0 is an anonymous msgADDR BASE-CLASS member at offset 0, hence the
// inheritance below rather than a named `base` member). The SOLE definition of this type as of
// 2026-08-07: hcex/hcex_kinect_boundary.h used to carry a flat plain-C copy with opaque helper
// value-types and now includes this header instead (odr_dup drain). Helper value-type layouts
// filled from DB types_members 2026-08-04 (inpKINECT_SKEL 332, inpKINECT_ARM_HANDLES 28,
// gsINP_KINECT_ARM 16, gsINP_KINECT_UI_FRAME 20, EVENT_DISP_gsINP_KINECT 8).
//
// `state` bits, merged from the boundary header's provenance (read by
// hcex_change_kinect_state @0x823D1650): 0x1 == load pending, 0x2 == unload pending.

struct gsINP_KINECT_GESTURE_BASE; // boundary — pointer only
struct gsINP_KINECT_UI_ELEMENT;   // boundary — pointer only (UI overlay element)
struct inpKINECT;                 // boundary — physical Kinect device (see gsINP_KINECT_boundary.h)

struct gsINP_KINECT : public msgADDR {
    struct EVENT_DISP_gsINP_KINECT {          // DB: gsINP_KINECT::EVENT_DISP_gsINP_KINECT, 8 bytes
        dsEVENT_HANDLER handler;              // 0x0 — DB row is the unnamed dsEVENT_HANDLER base slot
        gsINP_KINECT   *pObj;                 // 0x4
    } eventDispatcher; // 0x020
    struct inpKINECT_SKEL {                   // DB: inpKINECT_SKEL, 332 bytes
        enum JOINT_TRACKING_STATE {           // DB: inpKINECT_SKEL::JOINT_TRACKING_STATE
            JOINT_POSITION_NOT_TRACKED = 0,
            JOINT_POSITION_INFERRED    = 1,
            JOINT_POSITION_TRACKED     = 2,
        };
        m3dV position;                        // 0x000
        m3dV jointPositions[20];              // 0x00C
        JOINT_TRACKING_STATE jointTrackingState[20]; // 0x0FC — 4-byte enum slots
    }                                                      primarySkel;    // 0x028
    struct inpKINECT_SKEL                                  secondarySkel;  // 0x174
    struct inpKINECT_ARM_HANDLES {            // DB: inpKINECT_ARM_HANDLES, 28 bytes
        bool isLeftArmEnabled;                // 0x00
        bool isRightArmEnabled;               // 0x01
        unsigned char _pad02[2];              // 0x02
        m3dV leftArmPos;                      // 0x04
        m3dV rightArmPos;                     // 0x10
    }                                                      primaryArms;    // 0x2C0
    struct inpKINECT_ARM_HANDLES                           secondaryArms;  // 0x2DC
    int                                                    primaryIdx;     // 0x2F8
    int                                                    secondaryIdx;   // 0x2FC
    dsTSTRING<wchar_t>                                     curGrammarFile; // 0x300
    dsVECTOR<gsINP_KINECT_GESTURE_BASE *, 8>               gestures;       // 0x304
    struct gsINP_KINECT_ARM {                 // DB: gsINP_KINECT_ARM, 16 bytes
        m2dV  position;                       // 0x0
        float timeHovering;                   // 0x8
        dsTSTRING<char> activeElementName;    // 0xC
    }                                                      uiInput;        // 0x318
    struct gsINP_KINECT_UI_FRAME {            // DB: gsINP_KINECT_UI_FRAME, 20 bytes
        dsVECTOR<gsINP_KINECT_UI_ELEMENT *, 8> elements;   // 0x0
    }                                                      uiFrame;        // 0x328
    bool                                                   uiIsKinectMode; // 0x33C
    unsigned char                                          _pad33D[3];     // 0x33D
    inpKINECT                                             *kinectDevice;   // 0x340
    apSTATE_T<unsigned long>                               state;          // 0x344 load/unload state bits
    float                                                  timerSpeechActive; // 0x348

    // 0x826EE930 — enable speech recognition for the current grammar, register the
    // uiSetKinectMicrophoneState event once, signal it inactive, and switch to the system rule set.
    // Reversed in gsINP_KINECT__Load.cpp.
    void Load();
    // 0x826EEA50 — disable speech recognition, register the event once, and signal the microphone
    // state as "none". Reversed in gsINP_KINECT__Unload.cpp.
    void Unload();
};
