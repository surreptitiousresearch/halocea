#pragma once
#include "../anim/anim_blend_SEQ_INFO.h"  // anim_blend::SEQ_ID_FULL
#include "../ds/dsCONST_ARRAY.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsTSTRING.h" // dsTSTRING<char> is a template used below (GetAnimSeq/Start)
#include "../m3d/m3dSPL.h"
// ws-engine mdl: lip-sync driver. Blends up to 5 phoneme animation sequences over time,
// sampling each sequence's spline to produce a per-phoneme blend coefficient.
// DB-verified layouts (types_members mdlLIP_SYNCER / ::PHONEME_INFO / ::STATUS /
// mdlLIP_SYNCER_vtbl / mdlLIP_SYNC_IFACE).

struct animSEQ;   // animSEQ.h
struct animINST;  // animINST.h
struct objOBJ;    // objOBJ.h
struct mdlLIP_SYNCER;

// Abstract interface base (single vtable pointer). DB size 4. Canonical def in mdlLIP_SYNC_IFACE.h.
#include "mdlLIP_SYNC_IFACE.h"

// Virtual dispatch table. DB-verified (types_members mdlLIP_SYNCER_vtbl).
struct mdlLIP_SYNCER_vtbl {
    void (__fastcall *dtr_mdlLIP_SYNCER)(mdlLIP_SYNCER *self);
    void (__fastcall *ApplyLipSync)(mdlLIP_SYNCER *self);
    void (__fastcall *OnReset)(mdlLIP_SYNCER *self);
    void (__fastcall *GetAnimSeq)(mdlLIP_SYNCER *self, const struct dsTSTRING<char> *name,
                                  anim_blend::SEQ_ID_FULL *seqOut, animSEQ **pSeqOut);
};

struct mdlLIP_SYNCER : mdlLIP_SYNC_IFACE {
    // Per-phoneme blend slot. DB-verified (types_members mdlLIP_SYNCER::PHONEME_INFO), size 20.
    struct PHONEME_INFO {
        anim_blend::SEQ_ID_FULL seq;    // 0x00 sequence identifier
        animSEQ                *pSeq;   // 0x08 resolved sequence
        float                   coef;   // 0x0C current blend coefficient
        m3dSPL                 *spline; // 0x10 amplitude spline over the sequence
    };

    // Driver state machine. DB-verified (types_enum_values mdlLIP_SYNCER::STATUS).
    enum STATUS {
        OFF      = 0,
        EASE_ON  = 1,
        EASE_OFF = 2,
        ACTIVE   = 3,
    };

    // __vftable inherited from mdlLIP_SYNC_IFACE at 0x00, but this class uses its own vtable type.
    dsCONST_ARRAY<PHONEME_INFO, 5> seqList;        // 0x04 up to 5 phoneme slots (index 4 = "closed")
    dsVECTOR<objOBJ *, 8>          objList;        // 0x6C target objects
    float                          timeEaseCur;    // 0x80 current ease blend time
    float                          timeCur;        // 0x84 current playback time
    float                          timeEnd;        // 0x88 total duration
    bool                           isClosedOnly;   // 0x8C only drive the "closed" slot
    bool                           useClosedAsIdle;// 0x8D
    animINST                      *pInst;          // 0x90
    int                            sampleRate;     // 0x94
    STATUS                         status;         // 0x98

    // 0x827473E0 — per-frame advance of the lip-sync state machine.
    void Update(float dt);

    // 0x82746F08 — default ctor: clears the 5 phoneme slots, empties objList, resets timers and
    // sampleRate (44100), status = OFF.
    mdlLIP_SYNCER();

    // 0x82747018 — begin a lip-sync clip: open "<voPath>/<name>.spl" (or the exclamation path),
    // load per-phoneme splines + sampleRate from its chunks, and compute timeEnd from the closed
    // slot's last keypoint. Transitions OFF/EASE_OFF -> EASE_ON.
    void Start(const dsTSTRING<char> &name, bool isExclamation);

    // 0x827495D8 (vtbl slot 1) — apply the current phoneme blend to every driven face object.
    void ApplyLipSync();

    // Reset transient blend state (vtbl slot OnReset); body external to this batch. boundary
    void OnReset();
};
