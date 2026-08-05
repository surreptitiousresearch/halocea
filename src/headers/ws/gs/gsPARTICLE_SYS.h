#pragma once
// ws-engine gs: base particle-system class hcexPARTICLE_SYS derives from. DB-verified layout
// (types_members gsPARTICLE_SYS) — size 4 (vtable pointer only). Out of scope for this batch
// beyond the vtable slot / base method hcexPARTICLE_SYS's methods touch.

struct gsPARTICLE_SYS;
struct rendCAM_LIST;
struct camCAMERA;
struct txmTEXTURE;
struct m3dMATR;
enum PART_REND_LAYER : int;

typedef struct gsPARTICLE_SYS_vtbl {
    void (*dtr_gsPARTICLE_SYS)(struct gsPARTICLE_SYS *self, int freeMemory);        // 0x00 deleting dtor
    void (*Prepare)(struct gsPARTICLE_SYS *self, rendCAM_LIST *, float);            // 0x04
    void (*Update)(struct gsPARTICLE_SYS *self, int, rendCAM_LIST *, float, int, int, int); // 0x08
    void (*Render)(struct gsPARTICLE_SYS *self, camCAMERA *, int, int, PART_REND_LAYER, txmTEXTURE *); // 0x0C
    void (*RenderShadowQueries)(struct gsPARTICLE_SYS *self, int, int, const m3dMATR *, const m3dMATR *, int); // 0x10
    void (*EndFrame)(struct gsPARTICLE_SYS *self);                                  // 0x14
    void (*TermLevel)(struct gsPARTICLE_SYS *self);                                 // 0x18
    void (*PrepareDelayedCDT)(struct gsPARTICLE_SYS *self);                         // 0x1C
    void (*PushDelayedCDT)(struct gsPARTICLE_SYS *self);                            // 0x20
    void (*StopDelayedCDT)(struct gsPARTICLE_SYS *self, int);                       // 0x24
} gsPARTICLE_SYS_vtbl;

typedef struct gsPARTICLE_SYS {
    static gsPARTICLE_SYS_vtbl vftable; // `gsPARTICLE_SYS::`vftable'' — boundary, not decompiled
    gsPARTICLE_SYS_vtbl *__vftable;    // 0x00

    // 0x82677AF0 — gsPARTICLE_SYS::PushDelayedCDT: clamp the pending-CDT count, resize the results
    // vector, pick a thread-mask count from gsMsgMTConfig and start the global partJobCdt job.
    // Reversed in gsPARTICLE_SYS__PushDelayedCDT.cpp. Concrete body operating over the file-scope
    // partJobCdt globals; `this` is unused.
    // NOTE (adjudicated 2026-08-04): 0x8290ECB0 is mid-body of dsVECTOR<WEAK_PTR<gsTROPHY_BASE>,8>
    // ::PushBack — a bogus attribution. The DB body ?PushDelayedCDT@gsPARTICLE_SYS@@UAAXXZ is at
    // 0x82677AF0, with vtable-slot data xrefs at 0x82034C00 / 0x822E8DD8.
    void PushDelayedCDT();

    // 0x82673A98 — stop the global partJobCdt job for a given thread id. Reversed in
    // gsPARTICLE_SYS__StopDelayedCDT.cpp.
    void StopDelayedCDT(int threadId);
} gsPARTICLE_SYS;
