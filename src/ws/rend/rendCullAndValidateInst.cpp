#include <stdint.h>
#include "../../headers/ws/rend/rendCAM_LIST.h"
#include "../../headers/ws/rend/rendCAM.h"
#include "../../headers/ws/rend/rendLIGHT_SET.h"
#include "../../headers/ws/anim/animINST.h"
#include "../../headers/ws/anim/animCULL_INFO.h"
#include "../../headers/ws/obj/objOBJ.h"
#include "../../headers/ws/obj/objGEOM_UNSHARED.h"
#include "../../headers/ws/obj/objGEOM_SHARED.h"
#include "../../headers/ws/cam/camCAMERA.h"
#include "../../headers/ws/m3d/m3dBOX.h"
#include "../../headers/ws/m3d/m3dOBB.h"
#include "../../headers/ws/m3d/m3dV.h"
#include "../../headers/ws/m3d/m4dV.h"
#include "../../headers/ws/m3d/m3d_boundary.h"
#include "../../headers/ws/ds/dsSMART_PTR.h"
#include "../../headers/ws/ds/dsREF_COUNT.h"
#include "../../headers/ws/wb/dbgVAR_boundary.h"
#include <string.h>

// rendCullAndValidateInst @ 0x8273CD68
//
// Per-frame per-instance visibility + validation pass. For the animated instance `pInst` it
// walks every render camera in `camList`, builds a per-camera visibility bitmask in
// pInst->cullInfo.camMask (with per-camera distances cached in cullInfo.camDistList), then — if
// the instance is visible to at least one camera — validates it, applies LOD, refreshes its
// light set, and (for skinned/composite instances) qualifies each sub-object per camera into
// pInst->pObjCamMask.
//
// Transcription notes (large body flagged for faithful transcription over exhaustive naming):
//  * ABI: the DB/decompiler mis-attributed the parameter registers. The disasm prologue
//    (0x8273CD8C-0x8273CD9C) shows r3=camList, r4=curFrameNmb, r6=the instance, f1=elapsedTime;
//    r5 is unused. Corrected to that real signature.
//  * The decompiler register-puns several 64-bit locals (a single register aliasing both a
//    mask's low dword and a pointer's high dword). Those are split here into separate typed
//    variables (objMask + box pointer, camMask bit shifts) — this is the main deviation and it
//    is behavior-preserving.
//  * Camera-distance reads the decompiler writes as `*((float*)&camMask + N)` are the
//    cullInfo.camDistList[] entries (camMask@0 is 8 bytes + isValid@8 is 4 bytes => camDistList
//    starts 3 floats in), resolved to camDistList[...] indexing.
//  * A thread-local "current instance" cache store (*(TLS+56) = pInst at 0x8273CDA4) is a
//    boundary os-TLS access unrelated to the cull result; omitted with this note.

extern "C" dbgVAR_SIMPLE<bool, 1> dbg_isOffRenderAnim; // boundary (only .value read)
extern "C" void osPIXBeginEvent(const char *name); // boundary — profiler marker
extern "C" void osPIXEndEvent();                   // boundary

// Signature per the mangled name ?rendCullAndValidateInst@@YAXABVrendCAM_LIST@@KMPAVanimINST@@@Z
// and disasm 0x8273CD68 (r3=camList, r4=curFrameNmb, f1=elapsedTime, r6=pInst): 4 args in that
// order. The earlier draft's extra `int r5` phantom (the skipped GPR slot behind the f1 float) and
// trailing elapsedTime are removed.
void rendCullAndValidateInst(const rendCAM_LIST *camList, unsigned int curFrameNmb,
                             float elapsedTime, animINST *pInst)
{
    pInst->cullInfo.camMask = 0;
    objOBJ *pObj = pInst->pObj;
    animCULL_INFO *cull = &pInst->cullInfo;
    // (boundary) TLS current-instance cache store omitted here — see header note.

    if (!pObj)
        return;
    if ((pInst->state & 1) != 0 || (pInst->stateVis & 1) != 0)
        return;

    int state2 = pInst->state2;
    if (!((state2 & 0x400000) != 0 && (state2 & 0x200000) != 0 && !dbg_isOffRenderAnim.value))
        return;

    const dsCONST_ARRAY<rendCAM, 50> *camArray = &camList->list;
    m3dBOX *bbox = nullptr;
    m3dV closestScratch; // DistPoint's closest-point out param (result ignored by this caller)

    // ---- Pass 1: build the per-camera visibility mask ----
    for (int ci = 0; ci < camArray->nEntry; ++ci) {
        const rendCAM *cam = &(*camArray)[ci]; // operator[] yields const T& — take its address
        unsigned int camState = cam->state.state;
        int instState = pInst->state;
        bool passVisibility = false;
        bool doCommonTest = false;

        if (((camState >> 4) & 1) != 0) {
            // color camera
            if (instState & 0x8000)
                continue;
            if (((camState >> 5) & 1) == 0)
                doCommonTest = true;
            else
                passVisibility = (instState & 0x10000) == 0;
        } else {
            // shadow / auxiliary camera
            if (instState & 0x10000)
                continue;
            int src = cam->sourceColorCamIdx;
            if (src == 0 && (pInst->stateVis & 2) == 0)
                continue;
            if (src == 1 && (pInst->stateVis & 4) == 0)
                continue;
            if (src == 0 && (pInst->state2 & 0x80) != 0)
                continue;
            if (src != 1)
                doCommonTest = true;
            else
                passVisibility = (pInst->state2 & 0x100) == 0;
        }

        if (!doCommonTest && !passVisibility)
            continue;

        // common visibility test
        if (((camState >> 11) & 1) != 0)
            continue;

        if (instState & 0x4000) {
            // force-visible: mark and zero distance
            cull->camMask |= (1LL << ci);
            cull->camDistList[cam->originID] = 0.0f;
            continue;
        }

        if (!bbox)
            bbox = pInst->GetBBox(0);

        if (cam->cam.frustum.CullAABB(&bbox->bll.x)) {
            // AABB outside frustum — but a stateful camera still records the distance
            if ((cam->state.state & 1) != 0) {
                char originID = cam->originID;
                m3dOBB *obb = pInst->GetOBB(0);
                // DB DistPoint sig: (p, distMax, vClosest, float *dist, int *faceNmb) — distance
                // goes in arg4 (camDistList), faceNmb unused (arg4/arg5 were swapped in the draft).
                obb->DistPoint(&cam->cam.matrC2W.v[3], 2000000.0f, &closestScratch,
                               &cull->camDistList[originID], nullptr);
            }
            continue;
        }

        // AABB inside frustum — distance/LOD-visibility test against the OBB
        char originID = cam->originID;
        float maxDist = cam->maxDist;
        m3dOBB *obb = pInst->GetOBB(0);
        if (!obb->DistPoint(&cam->cam.matrC2W.v[3], maxDist, &closestScratch,
                            &cull->camDistList[originID], nullptr)) // dist=arg4, faceNmb=arg5
            continue;

        bool markVisible = true;
        if (((cam->state.state >> 2) & 1) != 0) {
            float distScale = 1.0f;
            if (cam->maxDist < 2000000.0f) {
                distScale = cull->camDistList[cam->originID] / cam->maxDistExt;
                if (distScale <= 0.5f)
                    distScale = 0.5f;
            }
            markVisible = (cull->camDistList[cam->sourceColorCamIdx] * distScale)
                          <= (double)(pInst->fVisDistFactor * cam->distVisFactor);
        }
        if (markVisible)
            cull->camMask |= (1LL << ci);
    }

    // ---- Pass 2: pick the LOD distance from the visible cameras ----
    float lodDist = 2000000.0f;
    for (int ci = 0; ci < camArray->nEntry; ++ci) {
        if (((1LL << ci) & cull->camMask) != 0) {
            const rendCAM *cam = &(*camArray)[ci]; // operator[] yields const T& — take its address
            float d = cull->camDistList[cam->sourceColorCamIdx];
            if (lodDist >= d)
                lodDist = d;
            if ((((*camArray)[cam->sourceColorCamIdx].state.state >> 12) & 1) != 0) // operator[] yields ref
                lodDist = 0.0f;
        }
    }

    pInst->LODApply(cull->camMask != 0, lodDist);

    if (!cull->camMask)
        return;

    pInst->lastFameVisible = curFrameNmb;
    pInst->Invalidate(2097184, 0); // 0x200020
    pInst->Validate(2097187);      // 0x200023
    pInst->UpdateSpeed(elapsedTime);

    dsSMART_PTR<rendLIGHT_SET, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > lightSet = pInst->GetLightSet();
    if (lightSet.pointee && lightSet.pointee->NeedUpdate(curFrameNmb))
        lightSet.pointee->Update(pInst, elapsedTime);

    // ---- Per-object qualification for skinned/composite instances ----
    if ((pInst->state2 & 0x10000) != 0) {
        osPIXBeginEvent("QualifyObjects");
        int64_t *pObjCamMask = pInst->pObjCamMask;
        objOBJ **objList = pInst->ppObjListRend;
        int nObjRend = pInst->nObjRend;
        memset(pObjCamMask, 0, 8 * pInst->nObj);

        for (int oi = 0; oi < nObjRend; ++oi) {
            objOBJ *obj = objList[oi];
            int64_t objMask = cull->camMask;
            float objDist[12]; // per-object per-camera distance scratch (v59)

            unsigned int bit = 1;
            for (int ci = 0; ci < camArray->nEntry; ++ci, bit <<= 1) {
                if ((bit & (unsigned int)objMask) == 0)
                    continue;

                const rendCAM *cam = &(*camArray)[ci]; // operator[] yields const T& — take its address
                unsigned int camState = cam->state.state;

                int rejectByProc = ((camState >> 4) & 1) != 0 ? (obj->stateProc & 0x100)
                                                              : (obj->stateProc & 0x200);
                if (rejectByProc)
                    objMask &= ~(int64_t)bit;
                if (((camState >> 8) & 1) != 0 && (obj->stateVis.state & 2) == 0)
                    objMask &= ~(int64_t)bit;
                if (((camState >> 9) & 1) != 0 && (obj->stateVis.state & 4) == 0)
                    objMask &= ~(int64_t)bit;

                if ((bit & (unsigned int)objMask) == 0)
                    continue;

                // resolve the object's world-space box (shared-geom AABB when static, otherwise
                // the enclosing box of its live OBB)
                m3dOBB objObb;
                m3dBOX enclBox;
                m3dBOX *box;
                if ((obj->state & 1) != 0) {
                    box = &obj->pGeom->pSharedGeom->bbox;
                } else {
                    obj->GetOBB(&objObb);
                    objObb.CalcEnclBox(&enclBox);
                    box = &enclBox;
                }

                bool cullObject;
                if (cam->cam.frustum.CullAABB(&box->bll.x)) {
                    cullObject = true;
                } else {
                    float maxDist = cam->maxDist;
                    const m4dV *camPos = &cam->cam.matrC2W.v[3];
                    int within = (obj->state & 1) == 0
                        ? objObb.DistPoint(camPos, maxDist, &closestScratch, &objDist[(int)cam->originID], nullptr) // dist=arg4
                        : box->DistPoint(camPos, maxDist, &closestScratch, &objDist[(int)cam->originID], nullptr);

                    cullObject = !within
                        || (((cam->state.state >> 2) & 1) != 0
                            && (obj->stateRend & 0x20) != 0
                            && (((cam->state.state >> 14) & 1) != 0
                                || (((1LL << cam->sourceColorCamIdx) & objMask) == 0
                                    && objDist[cam->sourceColorCamIdx] > 15.0f)
                                || objDist[(int)cam->originID] > (double)cam->maxDistExt));
                }
                if (cullObject)
                    objMask &= ~(int64_t)bit;
            }

            // (v58) clamp the primary/secondary camera distances into the shared range — the
            // result is a scratch the decompiler discards; kept for fidelity.
            float clampScratch;
            if ((objMask & 1) != 0)
                clampScratch = _m3dClamp(0.0f, 127.5f, objDist[0]);
            if ((objMask & 2) != 0 && camList->nColorCam > 1)
                clampScratch = _m3dClamp(0.0f, 127.5f, objDist[1]);
            (void)clampScratch;

            pObjCamMask[obj->id] = objMask;
        }
        osPIXEndEvent();
    }

    lightSet.DeletePointee();
}
