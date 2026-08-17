/* ?_AllocGeom@objOBJ@@QAAHXZ @0x82A7ECC0 */
#include <new>
#include "objOBJ.h"
#include "objGEOM_UNSHARED.h"
#include "objGEOM_SHARED.h"
// 0x82A7ECC0  ?_AllocGeom@objOBJ@@QAAHXZ
// Lazily create this object's geometry: allocate an objGEOM_UNSHARED wrapper (default field
// values) plus its objGEOM_SHARED payload (default ctor). Returns 1 if geometry already exists
// or was created, 0 on any allocation failure.

// Debug-tagged global operator new(size, file, line) — boundary (not reversed here).
void *operator new(size_t size, const char *file, unsigned int line);

int objOBJ::_AllocGeom()
{
    if (this->pGeom)
        return 1; // already has geometry

    const char *allocFile = "D:\\Projects\\code\\common\\src.sys\\objects\\Obj_alloc.cpp";

    objGEOM_UNSHARED *pGeom =
        (objGEOM_UNSHARED *)operator new(sizeof(objGEOM_UNSHARED), allocFile, 0x100);
    if (pGeom) {
        pGeom->stateDyn.state = 0;
        pGeom->alphaKillValue = 0;
        pGeom->pRendData = nullptr;
        pGeom->nModifiers = 0;
        pGeom->modList = nullptr;
        pGeom->color = 0xFFFFFFFF;
        pGeom->texOffset[0].s = 0.0f;
        pGeom->color_mp = 0;
        pGeom->texOffset[0].t = 0.0f;
        pGeom->renderPassId = 0;
        pGeom->texOffset[1].s = 0.0f;
        pGeom->blendRend = 0;
        pGeom->texOffset[1].t = 0.0f;
        pGeom->pSharedGeom = nullptr;
        pGeom->transp = 1.0f;
        pGeom->zBiasValue = 0;
        pGeom->lodTransp = 1.0f;
        pGeom->transpRendPriority = 0x80;
        pGeom->pSharingHostObj = nullptr;
        pGeom->sharingOfsVtx = 0;
        pGeom->sharingOfsFace = 0;
    }
    this->pGeom = pGeom;
    if (!pGeom)
        return 0;

    objGEOM_SHARED *pSharedGeom =
        (objGEOM_SHARED *)operator new(sizeof(objGEOM_SHARED), allocFile, 0x103);
    if (pSharedGeom)
        new (pSharedGeom) objGEOM_SHARED();
    // DEVIATION: the decompiler dropped the pointer carry on the constructed path (left the
    // stored pointer uninitialised); the disasm keeps pSharedGeom, so it is stored as allocated.
    this->pGeom->pSharedGeom = pSharedGeom;
    return pSharedGeom != nullptr;
}
