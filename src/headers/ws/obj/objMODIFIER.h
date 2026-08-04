#pragma once
#include "../ap/apSTATE_T.h"
// ws-engine obj: geometry modifier — a polymorphic hook attached to an object that can
// transform / veto its geometry each frame. DB-verified layout (types_members objMODIFIER,
// objMODIFIER_vtbl).

struct objOBJ;   // ../obj/objOBJ.h — pointer only
struct fioFILE;  // boundary — serialization stream, pointer only
struct objMODIFIER;

// Virtual table. DB-verified layout (types_members objMODIFIER_vtbl).
typedef struct objMODIFIER_vtbl {
    // 0x00 scalar-deleting destructor; the real ABI slot takes a hidden "should-free" flag
    // (deleteFlag bit0 => free), modeled directly here so callers dispatch without a cast.
    void (*dtr_objMODIFIER)(objMODIFIER *self, int deleteFlag);
    int  (*AddObj)(objMODIFIER *self, objOBJ *obj);  // 0x04
    void (*DelObj)(objMODIFIER *self);               // 0x08
    int  (*Modify)(objMODIFIER *self);               // 0x0C returns 0 to veto the batch
    void (*Write)(objMODIFIER *self, fioFILE *file); // 0x10
    void (*Read)(objMODIFIER *self, fioFILE *file);  // 0x14
} objMODIFIER_vtbl;

typedef struct objMODIFIER {
    objMODIFIER_vtbl         *__vftable;   // 0x00
    int                       prior;       // 0x04 apply priority
    apSTATE_T<unsigned long>  stateVldReq; // 0x08 required valid-state bits
    apSTATE_T<unsigned long>  stateVld;    // 0x0C valid-state mask
    apSTATE_T<unsigned long>  state;       // 0x10 current state (bit 0 = disabled)
    objOBJ                   *pObj;        // 0x14 owning object
} objMODIFIER;
