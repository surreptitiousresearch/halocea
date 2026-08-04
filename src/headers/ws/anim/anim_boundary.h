#include <stdint.h>

#pragma once
/* External/SDK extern boundary — partial by design, see .complete/ESCALATIONS.md */
// anim subsystem free-function / intrinsic boundaries referenced by the reversed anim
// methods but not themselves reversed here.

// Saber allocator shims (dl memory layer) — C linkage.
extern "C" void dlFree(void *ptr);
extern "C" char *dlStrdup(const char *ptr, const char *file, uint32_t line); // src is read-only (2026-07-31)

// Saber logging sink (?_apLog@@YAXPBDZZ). boundary.
void _apLog(const char *format, ...);

// m3d "positive infinity" float constant at 0x820013F4 (decompiler symbol M3D_INFINITY_302),
// used as the sentinel distance for the farthest LOD level. boundary.
extern const float M3D_INFINITY;

// os atomic increment (?osLockedIncrement@@YAHPAH@Z).
int osLockedIncrement(int *pValue);

// PPC single-precision square-root intrinsic (modeled as a boundary call).
float __fsqrts(float x);

// ---- animINST::FillLinearList / FreeCollidables free-function + allocator boundaries ----

struct objOBJ;   // obj subsystem model object (pointer only)  boundary
struct animINST; // animINST.h — owning instance (pointer only) boundary

// Debug reallocator the ws build links in (dl memory layer).
extern "C" void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line);

// obj helper used to build the flat render list (anim.cpp). boundary.
void objMakePtrListID(objOBJ *pObjRoot, objOBJ **ppList, int nObj); // boundary

// Havok collidable teardown bookkeeping. boundary.
void RemoveCollidablesAllocated();                 // ?RemoveCollidablesAllocated@@YAXXZ
extern int sizeMatrLT_COPY;                        // running byte total of copied LT matrices

// Havok collidable — only its deleting destructor is referenced here. boundary.
struct hkpCollidable;
void hkpCollidable_scalar_deleting_dtor(hkpCollidable *self, unsigned int freeFlag); // boundary

// Per-frame heap the collidable/matrix arrays are freed through: obtained from the engine root
// pointer held in the small-data anchor slot (r13[0]); the interface lives at root+0x2C0 and its
// vtable slot +1 is Free(self, ptr). Modeled as a boundary allocator. See FreeCollidables CAVEAT.
struct dlHEAP_IFACE;
typedef struct dlHEAP_IFACE_vtbl {
    void *__reserved0;                        // 0x00
    void (*Free)(dlHEAP_IFACE *self, void *p);// 0x04
} dlHEAP_IFACE_vtbl;
typedef struct dlHEAP_IFACE {
    dlHEAP_IFACE_vtbl *__vftable; // 0x00
} dlHEAP_IFACE;
extern unsigned char *apSysRoot;  // r13[0] — engine root; heap interface at +0x2C0
