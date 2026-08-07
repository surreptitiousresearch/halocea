#pragma once
#include "inpDEVICE.h"
#include "../ds/dsVECTOR.h"
// ws-engine inp: the input subsystem root — owns the list of attached physical devices.
// DB-verified layout (types_members inpINPUT): __vftable@0, deviceList@4 — size 24.

struct inpINPUT;

// DB-verified vtable (types_members inpINPUT_vtbl).
typedef struct inpINPUT_vtbl {
    void (*dtr_inpINPUT)(inpINPUT *self, int deleteFlag);                                  // 0x00 — deleting dtor: vftable+0x00 holds ??_GinpINPUT@@UAAPAXI@Z
    bool (*Init)(inpINPUT *self);                                          // 0x04
    void (*Term)(inpINPUT *self);                                          // 0x08
    void (*Clear)(inpINPUT *self);                                         // 0x0C
    void (*Update)(inpINPUT *self);                                        // 0x10
    void (*FlushBuffers)(inpINPUT *self);                                  // 0x14
    void (*Acquire)(inpINPUT *self, bool acquire);                         // 0x18
    bool (*SetInputWnd)(inpINPUT *self, void *hwnd, bool exclusive);       // 0x1C
    void (*SetVibration)(inpINPUT *self, float left, float right, int pad);// 0x20
} inpINPUT_vtbl;

typedef struct inpINPUT {
    inpINPUT_vtbl        *__vftable;  // 0x00 dispatch vtable
    dsVECTOR<inpDEVICE *, 8> deviceList; // 0x04 attached physical devices

    // 0x823C2380 — number of attached devices.
    int GetDeviceCount() const;
    // 0x823C9D18 — attached device at `id` (unchecked index into deviceList).
    inpDEVICE *GetDevice(int id) const;
} inpINPUT;
