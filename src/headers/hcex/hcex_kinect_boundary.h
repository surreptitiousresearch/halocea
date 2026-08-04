#pragma once
#include <stddef.h> /* wchar_t is not a builtin type in C */
/* Boundary declarations for the ws-engine Kinect input subsystem (gsINP_KINECT) touched by the
 * HCEX bridge's hcex_change_kinect_state. gsINP_KINECT is a large, self-contained engine class
 * (skeleton tracking, gesture list, UI overlay state); the member sequence below is the full
 * DB-verified layout (types_members gsINP_KINECT, 844 bytes). Only the load/unload `state`
 * bitfield is actually touched by reversed HCEX code; the helper value-types (skeletons, arm
 * handles, gesture/UI blocks) are kept as opaque nested definitions of their DB size in this
 * boundary echo — their full DB member layouts are modeled in the canonical C++ home,
 * src/headers/ws/inp/gsINP_KINECT.h (DB checked 2026-08-04).
 *
 * state bits (from hcex_change_kinect_state): 0x1 == load pending, 0x2 == unload pending. */

#include "../ws/msg/msgADDR.h"
#include "../ws/ap/apSTATE_T.h"
#include "../ws/ds/dsTSTRING.h"
#include "../ws/ds/dsVECTOR.h"

struct gsINP_KINECT_GESTURE_BASE;   /* boundary — pointer only */
struct inpKINECT;                   /* boundary — pointer only */

typedef struct gsINP_KINECT
{
    struct msgADDR base;                                    /* 0x000 — msgADDR base */
    /* helper value-types below are boundary-opaque (DB sizes; internals not decompiled) */
    struct EVENT_DISP_gsINP_KINECT { unsigned char _opaque[8]; } eventDispatcher; /* 0x020 */
    struct inpKINECT_SKEL { unsigned char _opaque[332]; }  primarySkel;    /* 0x028 */
    struct inpKINECT_SKEL                                  secondarySkel;  /* 0x174 */
    struct inpKINECT_ARM_HANDLES { unsigned char _opaque[28]; } primaryArms;   /* 0x2C0 */
    struct inpKINECT_ARM_HANDLES                           secondaryArms;  /* 0x2DC */
    int                                                    primaryIdx;     /* 0x2F8 */
    int                                                    secondaryIdx;   /* 0x2FC */
    dsTSTRING<wchar_t>                                     curGrammarFile; /* 0x300 */
    dsVECTOR<gsINP_KINECT_GESTURE_BASE *, 8>               gestures;       /* 0x304 */
    struct gsINP_KINECT_ARM { unsigned char _opaque[16]; } uiInput;        /* 0x318 */
    struct gsINP_KINECT_UI_FRAME { unsigned char _opaque[20]; } uiFrame;   /* 0x328 */
    bool                                                   uiIsKinectMode; /* 0x33C */
    unsigned char                                          _pad33D[3];     /* 0x33D */
    inpKINECT                                             *kinectDevice;   /* 0x340 */
    apSTATE_T<unsigned long>                               state;          /* 0x344 load/unload state bits */
    float                                                  timerSpeechActive; /* 0x348 */
} gsINP_KINECT;                                            /* 844 bytes */

extern gsINP_KINECT *gsInpKinect;

/* 0x826EE930 / 0x826EEA50 — boundary, external to this batch. */
extern void gsINP_KINECT_Load(gsINP_KINECT *self);
extern void gsINP_KINECT_Unload(gsINP_KINECT *self);
