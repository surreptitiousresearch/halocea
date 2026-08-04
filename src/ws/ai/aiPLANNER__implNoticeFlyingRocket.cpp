#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiGRASS.h"
#include "../../headers/ws/ia/iaIACTOR.h"
#include "../../headers/ws/ds/dsTYPE_ID.h"

struct entENTITY;
// ?gssIsPlayer@@YAHPBVentENTITY@@@Z @ 0x829DF450 — true if the entity is a player. boundary.
extern int gssIsPlayer(const entENTITY *ent);

// pjl (projectile logic) descriptor RTTI id (?TYPE_ID@pjlPJL_DESC@@2VdsTYPE_ID@@A). boundary data.
struct pjlPJL_DESC { static dsTYPE_ID TYPE_ID; };

// aiPLANNER::implNoticeFlyingRocket @ 0x83183870
// ?implNoticeFlyingRocket@aiPLANNER@@UAAXPAViaIACTOR@@@Z  (virtual)
//
// A projectile actor is in flight. If it is a pjl projectile whose owner entity is the local
// player, raise the "grass" rocket-awareness counter (grass @ aiPLANNER+0x1128) to 2 frames.
void aiPLANNER::implNoticeFlyingRocket(iaIACTOR *actor)
{
    dsTYPE_ID typeId = actor->TypeID();
    if (!typeId.IsDerivedFrom(pjlPJL_DESC::TYPE_ID))
        return;

    // The projectile's owner-holder pointer sits at actor+0x1F8; its first field is the owner
    // entity. (Decompiler artifact: actor[2].propContainer.pPropOwner.)
    void *ownerHolder = *reinterpret_cast<void **>(reinterpret_cast<char *>(actor) + 0x1F8);
    const entENTITY *owner =
        ownerHolder ? *reinterpret_cast<const entENTITY **>(ownerHolder) : nullptr;

    if (owner && gssIsPlayer(owner))
        (&this->grass)->cntFramesRocket = 2;
}
