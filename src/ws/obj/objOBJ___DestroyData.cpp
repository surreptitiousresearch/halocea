/* ?_DestroyData@objOBJ@@QAAXXZ @0x82661B08 */
#include "objOBJ.h"
#include "objGEOM_UNSHARED.h"
#include "objGEOM_SHARED.h"
#include "objSPLIT.h"
#include "obj_boundary.h"
#include "../rend/rendORD.h"
#include "../ps/psSECTION.h"
#include "../ds/dsAFFIX_STRING.h"
#include "../ds/dsTSTRING_BUF_HEADER.h"
// 0x82661B08  ?_DestroyData@objOBJ@@QAAXXZ
// Tear down all data owned by this model-object: release its render data, then free every
// geometry buffer this object owns (a buffer is owned when its "shared" bit in stateShare is
// clear), reset the section handle and affix string, drop modifiers, and free the geometry
// wrapper. Called from the objOBJ destructor / re-init path.
//
// CAVEAT: the ownership tests read the 64-bit stateShare.state via `ld`/`sld`/`and` (verified
// in disasm). The decompiler rendered the tangent-list mask base as 0x80000010000LL, but the
// binary loads it with `lis r27,1` == 0x10000 (the high 0x80000000000 half is a phantom stale
// upper register word) — corrected below. The trailing `this->ps = psSECTION()` and
// `this->affixes = dsAFFIX_STRING()` resets were emitted as inlined temp-construct + assign +
// destruct; the affix reset's ref-counted buffer swap is reproduced explicitly.

void objOBJ::_DestroyData()
{
    dlFree(this->pMatrLTPrev);
    this->pMatrLTPrev = nullptr;

    objGEOM_UNSHARED *pGeom = this->pGeom;
    if (pGeom) {
        // Release the render-order payload (virtual deleting destructor, vtable slot 0).
        rendORD *pRendData = pGeom->pRendData;
        if (pRendData)
            delete pRendData;
        this->pGeom->pRendData = nullptr;

        objGEOM_SHARED *pShared = this->pGeom->pSharedGeom;

        if ((this->stateShare.state & 0x40) == 0)
            this->FreeObjData(1, pShared->vertBoneWeightList);
        if ((this->stateShare.state & 0x80) == 0)
            this->FreeObjData(2, pShared->vertBoneIdxList);

        bool instShared = (this->stateShare.state & 0x200000) != 0;
        this->state &= ~0xCu; // clear geometry-present bits 0x4|0x8 (bits unnamed in DB/PDB)
        if (!instShared)
            this->FreeObjData(35, pShared->pDataInst);

        if ((this->stateShare.state & 0x400000) == 0)
            this->FreeObjData(36, pShared->pDataInterleaved);

        if ((this->stateShare.state & 2) == 0)
            this->FreeObjData(0, pShared->vertList);
        this->nVert = 0;

        if ((this->stateShare.state & 1) == 0)
            this->FreeObjData(37, pShared->faceList);
        this->nFace = 0;

        // Texture-coordinate lists (bit 0x800<<i) and tangent lists (bit 0x10000<<i).
        for (int tc = 0; tc < 5; ++tc) {
            if ((this->stateShare.state & (0x800LL << tc)) == 0)
                this->FreeObjData(tc + 15, this->GetTexCoordList(tc));
            if ((this->stateShare.state & (0x10000LL << tc)) == 0)
                this->FreeObjData(tc + 4, pShared->tangentList[tc]);
        }

        if ((this->stateShare.state & 8) == 0)
            this->FreeObjData(3, pShared->normalVertList);

        // Vertex-color lists (bit 0x100<<i).
        for (int vc = 0; vc < 3; ++vc) {
            if ((this->stateShare.state & (0x100LL << vc)) == 0)
                this->FreeObjData(vc + 9, pShared->colorVertList[vc]);
        }

        // Render-batch split (bit 0x4).
        if ((this->stateShare.state & 4) == 0) {
            objSPLIT *pSplit = pShared->pSplit;
            if (pSplit)
                delete pSplit;
        }
    }

    this->_FreeName();

    // this->ps = psSECTION()  — reset the section handle to empty.
    {
        psSECTION emptySection;
        this->ps = emptySection;
    }

    // this->affixes = dsAFFIX_STRING()  — reset to the shared empty-string buffer. The inlined
    // dsTSTRING copy-assignment (self-assign guarded) and the temp's destruction are reproduced.
    {
        dsAFFIX_STRING emptyAffix;
        dsTSTRING_BUF_HEADER<char> *newBuffer = emptyAffix.str.pBuffer;
        if (&emptyAffix.str != &this->affixes.str) {
            dsTSTRING_BUF_HEADER<char> *oldBuffer = this->affixes.str.pBuffer;
            if (--oldBuffer->refCount == 0)
                dlFree(oldBuffer);
            this->affixes.str.pBuffer = newBuffer;
            ++newBuffer->refCount;
        }
        // ~dsAFFIX_STRING(emptyAffix): drop the temp's reference to the empty buffer.
        dsTSTRING_BUF_HEADER<char> *tempBuffer = emptyAffix.str.pBuffer;
        if (--tempBuffer->refCount == 0)
            dlFree(tempBuffer);
    }

    this->RemoveModifiers();
    this->_FreeGeom();
}
