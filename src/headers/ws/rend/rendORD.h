#pragma once
#include "../ds/dsSMART_PTR.h"
#include "../ds/dsSHARED_PTR.h"
#include "../ds/dsREF_COUNT.h"
#include "../ds/Deleter.h"
#include "rendDYN_LIGHT_STATE.h"
// ws-engine rend: render-order / render-data payload attached to a model-object's geometry
// (objGEOM_UNSHARED::pRendData). DB-verified layout (types_members rendORD): __vftable@0,
// pLightSet@4 (dsSMART_PTR<rendLIGHT_SET,_dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT*>>, 4B),
// dynLightsState@8 (rendDYN_LIGHT_STATE, 24B), dynLightsMask@32 (unsigned int[2], 8B),
// texInfoCache@40 (dsSHARED_PTR<rendTEXTURE_INFO_CACHE,0,Deleter<rendTEXTURE_INFO_CACHE>>, 8B),
// sdrLOD@48 (char[2], 2B) — size 50 (rounds to 52 as a base subobject, per vidORD's offsets).
// rendLIGHT_SET / rendTEXTURE_INFO_CACHE stay pointer-only (owned via the smart-pointer wrappers).

struct objOBJ;
struct rendLIGHT_SET;        // ../rendLIGHT_SET.h — pointer only (via dsSMART_PTR)
struct rendTEXTURE_INFO_CACHE; // boundary — pointer only (via dsSHARED_PTR)

struct rendORD;

// DB-verified (types_members rendORD_vtbl): dtor@0, Duplicate@4, GetSize@8.
typedef struct rendORD_vtbl {
    void (*dtr_rendORD)(rendORD *self);          // 0x00
    int  (*Duplicate)(rendORD *self, objOBJ *);  // 0x04
    int  (*GetSize)(rendORD *self);              // 0x08
} rendORD_vtbl;

typedef struct rendORD {
    rendORD_vtbl *__vftable; // 0x00
    // Virtual deleting destructor (vtable slot 0). Releases the render-order payload; the
    // objGEOM teardown invokes it as `delete pRendData`. Body lives in the rend subsystem (boundary).
    ~rendORD(); // boundary (vtable slot 0; vtable modeled explicitly above)

    // Virtual (vtable slot 1). Deep-copy this render-order data onto `dstObj`'s geometry.
    // Returns nonzero on success. Body lives in the rend subsystem (boundary).
    int Duplicate(objOBJ *dstObj); // boundary (vtable slot 1)

    // Virtual (vtable slot 2). Serialized/packed size of this render-order data. boundary.
    int GetSize(); // boundary (vtable slot 2)

    dsSMART_PTR<rendLIGHT_SET, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *>> pLightSet; // 0x04
    rendDYN_LIGHT_STATE dynLightsState;                                     // 0x08
    unsigned int        dynLightsMask[2];                                   // 0x20
    dsSHARED_PTR<rendTEXTURE_INFO_CACHE, 0, Deleter<rendTEXTURE_INFO_CACHE>> texInfoCache; // 0x28
    char                sdrLOD[2];                                          // 0x30
} rendORD;
