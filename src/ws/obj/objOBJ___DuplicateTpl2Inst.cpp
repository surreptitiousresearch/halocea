#include <string.h>
#include <new>
#include "objOBJ.h"
#include "objGEOM_UNSHARED.h"
#include "objGEOM_SHARED.h"
#include "../rend/rendORD.h"
// 0x82661600  ?_DuplicateTpl2Inst@objOBJ@@QAAPAV1@H@Z
// Duplicate this "template" object into a new instance. Copy-assign all fields, detach the clone from
// any hierarchy/instance, then (if this has geometry) make the clone share this template's shared
// geometry: allocate an unshared wrapper, drop its fresh shared payload, shallow-copy this wrapper
// (so pSharedGeom is shared), set up the geometry-sharing host/offsets, and recompute the clone's
// stateShare flags from which resources are present. When isShareGeom is false, the clone is fully
// unshared into a standalone object. Returns nullptr on allocation / rend-duplicate failure.
//
// CAVEAT: the decompiler packed the stateShare OR-masks into HIDWORD/LODWORD slots; the shift
// constants (texcoord 0x800<<tc, tangent 0x10000<<tc) are decoded from the disasm at 0x826617AC+.

// Global operator delete used to release the freshly-allocated shared geom before sharing this's.
void operator delete(void *p) noexcept;

objOBJ *objOBJ::_DuplicateTpl2Inst(int isShareGeom)
{
    objOBJ *dup = objOBJ::Make();
    if (!dup)
        return nullptr;

    *dup = *this; // objOBJ::operator=

    int state = dup->state;
    unsigned __int16 stateRend = dup->stateRend;
    dup->next = dup;
    dup->prev = dup;
    dup->child = nullptr;
    dup->parent = nullptr;
    dup->pGeom = nullptr;
    dup->pMatrLTPrev = nullptr;
    dup->state = state & ~0x10u;          // clear state bit 0x10 (bit unnamed in DB)
    dup->stateRend = stateRend & 0xF8FF;  // clear stateRend bits 0x0700
    dup->stateShare.state |= 0x20;        // mark as an instance clone

    if (this->pGeom) {
        dup->_AllocGeom();
        operator delete(dup->pGeom->pSharedGeom);                     // free the fresh shared payload
        memcpy(dup->pGeom, this->pGeom, sizeof(objGEOM_UNSHARED));    // share this template's pSharedGeom
        dup->stateShare.state |= 0x10;

        objGEOM_UNSHARED *pGeom = this->pGeom;
        if (pGeom && pGeom->pSharingHostObj)
            dup->SetGeomSharingHost(nullptr);
        dup->SetGeomSharingHost(this);
        dup->SetGeomSharingOfsVtx(this->GetGeomSharingsOfsVtx());
        dup->SetGeomSharingOfsFace(this->GetGeomSharingsOfsFace());
        dup->stateShare.state |= 0xF;

        if (dup->GetVertBoneWeightList(0))
            dup->stateShare.state |= 0x40;
        if (dup->GetVertBoneIdxList(0))
            dup->stateShare.state |= 0x80;
        if (dup->pGeom->pSharedGeom->pDataInst)
            dup->stateShare.state |= 0x200000;
        if (dup->pGeom->pSharedGeom->pDataInterleaved)
            dup->stateShare.state |= 0x400000;

        for (int tcInd = 0; tcInd < 5; ++tcInd) {
            if (this->GetTexCoordList(tcInd))
                dup->stateShare.state |= ((unsigned __int64)0x800 << tcInd);   // shared texcoord tcInd
            if (this->pGeom->pSharedGeom->tangentList[tcInd])
                dup->stateShare.state |= ((unsigned __int64)0x10000 << tcInd); // shared tangent tcInd
        }

        if (this->pGeom->pSharedGeom->colorVertList[0])
            dup->stateShare.state |= 0x100;
        if (this->pGeom->pSharedGeom->colorVertList[1])
            dup->stateShare.state |= 0x200;
        if (this->pGeom->pSharedGeom->colorVertList[2])
            dup->stateShare.state |= 0x400;

        dup->pGeom->nModifiers = 0;
        dup->pGeom->modList = nullptr;
        dup->pGeom->pRendData = nullptr;

        if (isShareGeom) {
            rendORD *pRendData = this->pGeom->pRendData;
            if (pRendData && !pRendData->Duplicate(dup))
                return nullptr;
        }
    }

    if (!isShareGeom) {
        dup->pInst = nullptr;
        dup->animNmb = -1;
        dup->_UnshareAll();
    }

    return dup;
}
