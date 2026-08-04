#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsVECTOR.h"
#include "../../headers/ws/ds/dsTYPE_ID.h"
#include "../../headers/ws/m3d/m3dV.h"
#include "../../headers/ws/msg/msgSYSTEM.h"
#include "../../headers/ws/msg/msgTYPE_ITER.h"
#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/ws/prop/propCONTAINER.h"
#include "../../headers/ws/prop/propSMT.h"
#include "../../headers/ws/tur/turTURRET.h"
#include "../../headers/ws/ds/dsFLAGS.h"

// dsSPrintf — format into the supplied dsTSTRING<char> out-param (returned by pointer). boundary.
extern dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *out, const char *fmt, ...);
// m3d subsystem — zero vector constant (?m3dVZero@@3Um3dV@@A). boundary.
extern const m3dV m3dVZero;

// DEVIATION: the two predicates below are deep opaque-vtable dispatches the decompiler expresses as
// raw slot calls; modeled as honestly-named boundary calls (no fabricated layout).
//   smtQueryAvailableEntryPos: propSMT entry-descriptor virtual (properties.pData[0] vtbl slot 2) —
//     returns whether an entry slot is available and, if so, hands back its world position.
extern bool smtQueryAvailableEntryPos(propSMT *smt, m3dV *out);
//   aiBrainVisionAimGate: brain vision-module virtual (this+0x1E8 vtbl+0x28) — the "can see well
//     enough to aim without an explicit angle check" gate.
extern bool aiBrainVisionAimGate(aiBRAIN *brain);

// aiBRAIN::implFindBestTurret(float radius, bool isCheckAngles) @ 0x83285910
// ?implFindBestTurret@aiBRAIN@@...  (PPC struct-return: hidden first arg is the returned dsTSTRING)
//
// Return the name of the best turret smart-object within `radius`: run the "FindBestTurret" SSL
// common back-off gate, then iterate every entity registered with the message system, keep those
// that host a turTURRET whose entry point is available and (optionally, when `isCheckAngles`) whose
// barrel can aim at the brain's target point (turTURRET::CanAimAtPoint). Return the nearest kept
// turret's name (empty string when none / when gated off / when radius is ~0).

extern const dsTYPE_ID turTURRET_DESC_TYPE_ID;      // turTURRET_DESC::TYPE_ID      boundary
extern const dsTYPE_ID propSMT_DESC_TYPE_ID;        // propSMT_DESC::TYPE_ID        boundary
extern const dsTYPE_ID propCONTAINER_DESC_TYPE_ID;  // propCONTAINER_DESC::TYPE_ID  boundary

// Find the turTURRET property hosted anywhere in a container (self, properties, components).
static turTURRET *findTurret(propCONTAINER *pc)
{
    if (pc->typeId.IsDerivedFrom(turTURRET_DESC_TYPE_ID))
        return (turTURRET *)pc;
    if (!pc->typeId.IsDerivedFrom(propCONTAINER_DESC_TYPE_ID))
        return nullptr;
    for (int i = 0; i < pc->properties.nElem; ++i)
        if (turTURRET *t = pc->properties[i]->GetProperty<turTURRET>())
            return t;
    for (int i = 0; i < pc->components.nElem; ++i)
        if (turTURRET *t = pc->components[i]->GetProperty<turTURRET>())
            return t;
    return nullptr;
}

// Resolve the propSMT smart object hosted in the same container as the turret.
static propSMT *findSmartObject(propCONTAINER *pc)
{
    if (pc->typeId.IsDerivedFrom(propSMT_DESC_TYPE_ID))
        return (propSMT *)pc;
    if (!pc->typeId.IsDerivedFrom(propCONTAINER_DESC_TYPE_ID))
        return nullptr;
    for (int i = 0; i < pc->properties.nElem; ++i)
        if (propSMT *s = pc->properties[i]->GetProperty<propSMT>())
            return s;
    for (int i = 0; i < pc->components.nElem; ++i)
        if (propSMT *s = pc->components[i]->GetProperty<propSMT>())
            return s;
    return nullptr;
}

dsTSTRING<char> aiBRAIN::implFindBestTurret(float radius, bool isCheckAngles)
{
    // SSL common back-off gate.
    {
        dsTSTRING<char> callName;
        dsFLAGS<SSL_COMMON_BACKOFF, int> backoffFlags{3};
        if (IsSSLCommonBackOff(*dsSPrintf(&callName, "FindBestTurret(\"%f\")", radius), backoffFlags))
            return dsTSTRING<char>();   // empty
    }

    if (radius < 0.000001f)
        return dsTSTRING<char>();       // empty

    // Collect every candidate turret entity that can be aimed.  b8_turret.cpp:216/249 debug tags.
    dsVECTOR<entENTITY *, 8> candidates;

    // DEVIATION: the decompiler garbles the typed msg-iterator and the propSMT entry-availability /
    // position virtual dispatch (pData[2].pHandle / pData[2].spDesc.pointee). Modeled at the
    // observed-semantics level: iterate all entity endpoints and keep those whose turret's entry
    // point is available and (when isCheckAngles) can aim at the brain's target point.
    for (msgTYPE_ITER<entENTITY> it(msgSystem.Front()); !it.IsDone(); ++it)
    {
        entENTITY *ent = it.Get();
        if (!ent)
            continue;

        propCONTAINER *pc = &ent->propContainer;
        turTURRET *turret = findTurret(pc);
        if (!turret)
            continue;

        propSMT *smt = findSmartObject(pc);
        if (!smt)
            continue;

        // Smart object must currently expose an available entry-point slot (and hand back its
        // world position). Both are propSMT entry-descriptor virtuals — boundary.
        m3dV entryPos;
        if (!smtQueryAvailableEntryPos(smt, &entryPos))
            continue;

        // Aim gate: either not requested, or the brain's vision predicate passes, or the turret can
        // physically aim at the brain's target point (st.pointee target position @ +0x370). boundary.
        const m3dV *aimTarget = (const m3dV *)((const char *)st.pointee + 0x370);
        if (!isCheckAngles || aiBrainVisionAimGate(this) || turret->CanAimAtPoint(*aimTarget))
            candidates.PushBack(ent);
    }

    // Pick the nearest candidate.
    entENTITY *best = nullptr;
    float bestDist = 1000000.0f;
    for (int i = 0; i < candidates.nElem; ++i)
    {
        entENTITY *cand = candidates[i];
        m3dV center = m3dVZero;
        cand->GetCenter(&center);
        float d = this->Dist(&center);
        if (d < bestDist)
        {
            best = cand;
            bestDist = d;
        }
    }

    if (best)
        return best->GetName();
    return dsTSTRING<char>();            // empty
}
