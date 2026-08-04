#pragma once
#include "../ws/gs/gsPARTICLE_SYS.h"

// HCEX bridge: gsPARTICLE_SYS specialization that also kicks the engine's collide-detect-transform
// (CDT) job threads whenever a delayed CDT push is requested. DB-verified layout (types_members
// hcexPARTICLE_SYS): base gsPARTICLE_SYS @0 — size 4 (no extra fields).
struct rendCAM_LIST;
struct camCAMERA;
struct txmTEXTURE;
struct m3dMATR;
enum PART_REND_LAYER;

// Full DB vtable (types_members hcexPARTICLE_SYS_vtbl, 10 slots).
typedef struct hcexPARTICLE_SYS_vtbl {
    void (*dtr_hcexPARTICLE_SYS)(struct hcexPARTICLE_SYS *self, int freeMemory); // 0x00 deleting dtor
    void (*Prepare)(struct hcexPARTICLE_SYS *self, rendCAM_LIST *camList, float dt); // 0x04
    void (*Update)(struct hcexPARTICLE_SYS *self, int a, rendCAM_LIST *camList, float dt, int b, int c, int d); // 0x08
    void (*Render)(struct hcexPARTICLE_SYS *self, camCAMERA *cam, int a, int b, PART_REND_LAYER layer, txmTEXTURE *tex); // 0x0C
    void (*RenderShadowQueries)(struct hcexPARTICLE_SYS *self, int a, int b, const m3dMATR *m0, const m3dMATR *m1, int c); // 0x10
    void (*EndFrame)(struct hcexPARTICLE_SYS *self);          // 0x14
    void (*TermLevel)(struct hcexPARTICLE_SYS *self);         // 0x18
    void (*PrepareDelayedCDT)(struct hcexPARTICLE_SYS *self); // 0x1C
    void (*PushDelayedCDT)(struct hcexPARTICLE_SYS *self);    // 0x20
    void (*StopDelayedCDT)(struct hcexPARTICLE_SYS *self, int a); // 0x24
} hcexPARTICLE_SYS_vtbl;

struct hcexPARTICLE_SYS : gsPARTICLE_SYS {
    static hcexPARTICLE_SYS_vtbl vftable; // `hcexPARTICLE_SYS::`vftable'' — boundary, not decompiled

    // 0x823CF098 — install this class's vftable (no other state to init).
    hcexPARTICLE_SYS();

    // 0x823CF0A8 — reset __vftable to the gsPARTICLE_SYS base slot (no owned state to release).
    ~hcexPARTICLE_SYS(); // base dtor is not a C++ virtual (opaque boundary): no 'override'

    // 0x823D158C — compiler-generated vector deleting destructor (`??_E` mangle).
    hcexPARTICLE_SYS *vectorDeletingDtor(unsigned char deleteFlags);

    // 0x823CEF88 (overrides gsPARTICLE_SYS::PushDelayedCDT) — run the base engine's CDT push, then
    // kick the global partJobCdt job on the three CDT thread masks (2, 4, 16).
    void PushDelayedCDT(); // base slot is a vtable fn-ptr, not a C++ virtual: no 'override'
};
