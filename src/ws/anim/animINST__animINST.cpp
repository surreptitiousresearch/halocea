#include "animINST.h"
#include "../rend/rendLIGHT_SET.h"
#include "../ds/dsSMART_PTR.h"

// animINST::animINST() @ 0x825E3E60 (protected per mangle `I`)
// Default-construct: zero the identity/state/link fields, reset the visibility position and
// bounding markers to zero, set scale to (1,1,1), null every owned pointer, default-construct
// dynLightsState/affixes/ps (implicit member construction), mark idAnimCdt as "no collision
// layer" (-1), zero the render-list counters, initialize a named per-instance lock ("animINST",
// not yet inited, no locker recorded), reset idxLoaded/followTracker, zero the anim-call/logging
// counters, mark the modifier-hierarchy cache invalid (OBJMODCACHE_INVALID), tag the modifier
// cache vector's allocation cookie, and clear the two validity-state words.
// D:\Projects\code\common\src.sys\animation\anim.cpp:118 tags objectsWithModifiersCache's
// allocation cookie.
// CAVEAT: the decompile builds entSpeed via a `z` scratch local assigned mid-sequence (register
// shuffled ahead of prevPos/pSkel/pObj/nObj/ppObjList/fVisDistFactor/ps construction) purely to
// keep a live FPR value across those intervening stores -- not a real aliasing hazard. Written
// here as a plain `entSpeed = m3dVZero` assignment in its natural position.
extern const m3dV m3dVZero;   // boundary — zero-vector constant
extern const m3dV m3dVOneXYZ; // boundary — (1,1,1) scale constant

animINST::animINST()
{
    this->sign = 0;
    this->state = 0;
    this->state2 = 0;
    this->pEnt = nullptr;
    this->stateVis = 0;
    this->prevPosVis = m3dVZero;
    this->stateVld = -1;
    this->id = 0;
    this->name = nullptr;
    this->nameClass = nullptr;
    this->nameTpl = nullptr;
    this->prev = nullptr;
    this->next = nullptr;
    this->scale = m3dVOneXYZ;
    this->pAnimMng = nullptr;
    this->pObjRoot = nullptr;

    typedef dsSMART_PTR<rendLIGHT_SET, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > LIGHT_SET_SP;
    reinterpret_cast<LIGHT_SET_SP *>(&this->pLightSet)->pointee = nullptr;

    this->pPhysChar = nullptr;
    this->pPhysSystem = nullptr;
    this->cdtLayer = 0;
    this->idAnimCdt = -1;
    this->lastFameVisible = 0;
    this->obbExpansion = 0.0f;
    this->nObjRend = 0;
    this->ppObjListRend = nullptr;
    this->pObjCamMask = nullptr;
    this->isObjRendVis = false;
    this->prevPos = m3dVZero;
    this->entSpeed = m3dVZero;
    this->fVisDistFactor = 0.0f;
    this->pSkel = nullptr;
    this->pObj = nullptr;
    this->nObj = 0;
    this->ppObjList = nullptr;
    this->pTpl = nullptr;
    this->pLOD = nullptr;
    this->pScn = nullptr;
    this->pAuxData = nullptr;

    this->perInstLock.name = "animINST";
    this->perInstLock.isInited = true;
    this->perInstLock.threadNmb = -1;
    this->perInstLock.locker_file = nullptr;
    this->perInstLock.locker_line = -1;
    this->perInstLock.Init();

    this->idxLoaded = -1;
    this->followTracker = nullptr;
    this->nCallsClacLTM = 0;
    this->nCallsApplyAnim = 0;
    this->isCheckNeed = 1;
    this->isCheckReady = 0;
    this->nLTMLogged = 0;
    this->nAnimLogged = 0;
    this->objMOD_CACHE_STATE = OBJMODCACHE_INVALID;

    this->objectsWithModifiersCache.pData = nullptr;
    this->objectsWithModifiersCache.nElem = 0;
    this->objectsWithModifiersCache.allocated = 0;
    this->objectsWithModifiersCache.__cl.file = "D:\\Projects\\code\\common\\src.sys\\animation\\anim.cpp";
    this->objectsWithModifiersCache.__cl.line = 118;

    this->stateVldModHierReq.state = 0;
    this->stateVldModHier.state = 0;
    this->pCollidableData = nullptr;
}
