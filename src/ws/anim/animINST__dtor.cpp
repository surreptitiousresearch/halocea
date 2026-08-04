#include "animINST.h"
#include "animMNG.h"
#include "../obj/objOBJ.h"
#include "../rend/rendLIGHT_SET.h"
#include "../ds/dsSMART_PTR.h"

extern "C" void dlFree(void *ptr); // boundary — dlmalloc allocator free
void animSetObjInstRef(objOBJ *pObj, animINST *pInst); // boundary (0x82683xxx)

// this->pLightSet is stored as an opaque void* (see animINST.h); reinterpret it as the real
// smart-pointer type to reach .pointee/.DeletePointee(), matching animINST::AddLightSet's
// established convention for this field.
typedef dsSMART_PTR<rendLIGHT_SET, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > LIGHT_SET_SP;

// animINST::~animINST() @ 0x825E3908 (protected per mangle `I`)
// Detach from the owned light set (notifying it so it drops the back-reference), tear down LOD
// state, release the model object tree (detach-only if state2's 0x1000 bit is set -- meaning the
// objects outlive this instance -- otherwise fully delete them), free the render/camera-mask
// scratch arrays and the three owned name strings, invoke the anim manager's virtual "deleting
// destructor" slot (freeMemory=1, matching AddLightSet/LODInit's own-allocation convention), free
// the modifier cache, terminate the per-instance lock, destroy ps/affixes (implicit member
// dtors), then finally drop the light-set smart pointer's owned reference.
animINST::~animINST()
{
    LIGHT_SET_SP *pLightSet = reinterpret_cast<LIGHT_SET_SP *>(&this->pLightSet);
    rendLIGHT_SET *pointee = pLightSet->pointee;
    if (pointee)
        pointee->NotifyDestroyInst(this);

    this->LODTerm();

    objOBJ *pObj = this->pObj;
    if (this->state2 & 0x1000)
        animSetObjInstRef(pObj, nullptr);
    else
        objOBJ::Delete(pObj);

    dlFree(this->ppObjList);
    dlFree(this->ppObjListRend);
    dlFree(this->pObjCamMask);
    dlFree(this->name);
    dlFree(this->nameClass);
    dlFree(this->nameTpl);

    animMNG *pAnimMng = this->pAnimMng;
    if (pAnimMng)
        pAnimMng->__vftable->dtr_animMNG(pAnimMng, 1);

    dlFree(this->objectsWithModifiersCache.pData);
    this->perInstLock.Term();
    this->ps.~psSECTION();

    dsTSTRING_BUF_HEADER<char> *affixBuffer = this->affixes.str.pBuffer;
    if (affixBuffer->refCount-- == 1)
        dlFree(affixBuffer);

    pLightSet->DeletePointee();
}
