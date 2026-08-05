#pragma once
#include "../dsc/dscPRODUCT.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsPARAM_LIST.h"
#include "../anim/animINST.h"
// ws-engine gs: base class for a live "object modifier" instance driving one entry of a
// propOBJ_MODIFIER_LIST (e.g. an animation/state modifier attached to a game object).
// Non-polymorphic dscPRODUCT introduces no vtable of its own, so under the MSVC ABI the vfptr
// this class introduces is placed AHEAD of the dscPRODUCT sub-object.
// DB-verified layout (types_members gsOBJ_MODIFIER_BASE): __vftable@0, dscPRODUCT@4 — size 8.

struct gsOBJ_MODIFIER_BASE;

// DB-verified (types_members gsOBJ_MODIFIER_BASE_vtbl), slots 0-6.
typedef struct gsOBJ_MODIFIER_BASE_vtbl {
    void (*dtr_gsOBJ_MODIFIER_BASE)(gsOBJ_MODIFIER_BASE *self);                                  // 0x00
    void (*OnInit)(gsOBJ_MODIFIER_BASE *self, animINST *);                                       // 0x04
    void (*OnUpdate)(gsOBJ_MODIFIER_BASE *self, float);                                          // 0x08
    void (*OnTerm)(gsOBJ_MODIFIER_BASE *self);                                                    // 0x0C
    void (*OnStart)(gsOBJ_MODIFIER_BASE *self);                                                   // 0x10
    void (*OnStop)(gsOBJ_MODIFIER_BASE *self);                                                    // 0x14
    void (*OnEvent)(gsOBJ_MODIFIER_BASE *self, const dsTSTRING<char> *, const dsPARAM_LIST *);    // 0x18
} gsOBJ_MODIFIER_BASE_vtbl;

// Spelled with the vfptr and the dscPRODUCT sub-object as explicit members (in DB order:
// __vftable@0, dscPRODUCT base@4) because the vfptr this class introduces precedes the
// non-polymorphic base under the MSVC ABI — plain inheritance would invert the layout.
typedef struct gsOBJ_MODIFIER_BASE {
    gsOBJ_MODIFIER_BASE_vtbl *__vftable; // 0x00 introduced by this class; precedes dscPRODUCT per MSVC ABI
    dscPRODUCT                base;      // 0x04 dscPRODUCT sub-object

    ~gsOBJ_MODIFIER_BASE();                                                  // boundary — vtbl slot 0
    void OnInit(animINST *inst);                                            // boundary — vtbl slot 1
    void OnUpdate(float dt);                                                // boundary — vtbl slot 2
    void OnTerm();                                                          // boundary — vtbl slot 3
    void OnStart();                                                         // boundary — vtbl slot 4
    void OnStop();                                                          // boundary — vtbl slot 5
    // vtbl slot 6 — dispatch a named event with parameters to this modifier instance.
    void OnEvent(const dsTSTRING<char> *event, const dsPARAM_LIST *params); // boundary
} gsOBJ_MODIFIER_BASE;
