// entCREATE_DATA::entCREATE_DATA @0x8252FFB8 — default-construct: base animCREATE_DATA, then
// install entCREATE_DATA's own vtable and null pDomSpawn.
#include "../../headers/entCREATE_DATA.h"
#include "../../headers/ws/anim/animCREATE_DATA.h"

struct entCREATE_DATA_vtbl; // boundary — ent subsystem vtable
extern entCREATE_DATA_vtbl entCREATE_DATA_vftable; // `entCREATE_DATA::`vftable''

entCREATE_DATA::entCREATE_DATA() : animCREATE_DATA()
{
    // vtable slot typed as base animCREATE_DATA_vtbl*; install derived table via cast
    __vftable = (animCREATE_DATA_vtbl *)&entCREATE_DATA_vftable;
    pDomSpawn = nullptr;
}
