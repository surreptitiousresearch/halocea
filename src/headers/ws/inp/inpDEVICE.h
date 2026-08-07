#pragma once
// ws-engine inp: a single physical input device (keyboard/mouse/gamepad). Layout DB-verified
// (types_members); only `type` (used by dsVECTOR<inpDEVICE*,8>::Find<CMP,dsSTRID>) is typed,
// the rest are opaque padding at their real DB offsets since no reversed method touches them.

#include "../ds/dsSTRID.h"
#include "../ds/dsTSTRING.h"

struct inpFORCE_FEEDBACK_CTRL_BASE; // boundary, only pointed to here

// DB-verified (types_members inpDEVICE_vtbl, 16 slots / 64 bytes).
typedef struct inpDEVICE_vtbl
{
    void (*dtr_inpDEVICE)(struct inpDEVICE *self, int deleteFlag);                    // 0x00 — deleting dtor: vftable+0x00 holds ??_GinpDEVICE@@UAAPAXI@Z
    bool (*Init)(struct inpDEVICE *self);                             // 0x04
    void (*Term)(struct inpDEVICE *self);                             // 0x08
    void (*Clear)(struct inpDEVICE *self);                            // 0x0C
    void (*UpdateInput)(struct inpDEVICE *self);                      // 0x10
    void (*FlushBuffers)(struct inpDEVICE *self);                     // 0x14
    void (*SetLayout)(struct inpDEVICE *self, int);                   // 0x18
    bool (*SetInputWnd)(struct inpDEVICE *self, void *, bool);        // 0x1C
    int  (*GetKeyType)(struct inpDEVICE *self, int);                  // 0x20
    void (*ResetReleasedKeys)(struct inpDEVICE *self);                // 0x24
    void (*Acquire)(struct inpDEVICE *self, bool);                    // 0x28
    void (*SetVibration)(struct inpDEVICE *self, float left, float right, int enable); // 0x2C
    bool (*InitForceFeedBack)(struct inpDEVICE *self);                // 0x30
    bool (*GetKeyName)(struct inpDEVICE *self, int, wchar_t *);       // 0x34
    int  (*GetInstId)(struct inpDEVICE *self);                        // 0x38
    bool (*IsAcceptKeyPress)(struct inpDEVICE *self, int);            // 0x3C
} inpDEVICE_vtbl;

typedef struct inpDEVICE
{
    inpDEVICE_vtbl       *__vftable;             // 0x0
    dsTSTRING<wchar_t>    name;                  // 0x4
    float                 keyValue[340];         // 0x8
    unsigned char          keyState[340];         // 0x558
    int                    layout[340];           // 0x6AC
    float                  prevKeyValue[340];     // 0xBFC
    dsSTRID                type;                  // 0x114C
    inpFORCE_FEEDBACK_CTRL_BASE *forceFeedbackIface; // 0x1150 -- opaque boundary

    // ?GetKeyValue@inpDEVICE@@QBAMH@Z @ 0x8263EE30 — analog value of key/axis. Reversed in
    // src/ws/inp/inpDEVICE__GetKeyValue.cpp.
    float GetKeyValue(int key) const;
} inpDEVICE; // 4436 bytes
