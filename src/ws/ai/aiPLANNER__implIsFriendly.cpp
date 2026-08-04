#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ia/iaIACTOR.h"
#include "../../headers/ws/ds/dsTYPE_ID.h"
#include "../../headers/ws/gs/gsTEAMS.h"
#include "../../headers/ws/ds/dsSTRID.h"

// ap subsystem logging sink (printf-style). boundary.
extern void _apLog(const char *fmt, ...);

struct entENTITY; // ent subsystem — root game entity (pointers only here)   boundary (fwd)

// ent subsystem — RTTI derivation anchor for the entity class hierarchy. Local anchor (matches the
// established idiom in aiWATCHER_BRAIN::IsAcceptActor); the TYPE_ID static is defined out of line.
// boundary.
struct entDESC { static dsTYPE_ID TYPE_ID; };

// 0x83173B48 (?GetTeamIDFromEntity@@YA?AVdsSTRID@@PAVentENTITY@@@Z) — the team id an entity belongs
// to (sret). boundary — reversed as its own frontier.
extern dsSTRID GetTeamIDFromEntity(entENTITY *ent);

// aiPLANNER::implIsFriendly(iaIACTOR*, iaIACTOR*) @ 0x83173CD0
// ?implIsFriendly@aiPLANNER@@UAA_NPAViaIACTOR@@0@Z  (virtual)
// SSL predicate: are the two objects mutually neutral (each side rates the other neutral). Both
// arguments must be non-null entities; an object is friendly to itself.
bool aiPLANNER::implIsFriendly(iaIACTOR *obj1, iaIACTOR *obj2)
{
    if (!obj1)
    {
        _apLog("~AI,Teams,Ssl,Error~Attitude: can't call IsFriendly(): 1st parameter is NOT valid (object is NULL)");
        return false;
    }

    // obj1 must be an entity (its runtime type derives from entDESC).
    dsTYPE_ID typeId1;
    obj1->__vftable->TypeID(obj1, &typeId1);
    if (!typeId1.IsDerivedFrom(entDESC::TYPE_ID))
    {
        // DEVIATION: the original also formats the actor's dynamic name (a vtable-slot GetName)
        // into this diagnostic; omitted (name accessor is a boundary vtbl slot, log-only path).
        _apLog("~AI,Teams,Ssl,Error~Attitude: can't call IsFriendly(): 1st object is NOT valid (object is not entity)");
        return false;
    }

    if (!obj2)
    {
        _apLog("~AI,Teams,Ssl,Error~Attitude: can't call IsFriendly(): 2nd parameter is NOT valid (object is NULL)");
        return false;
    }

    if (!obj2->IsA<entENTITY>())
    {
        // DEVIATION: see above — dynamic-name argument to this diagnostic omitted.
        _apLog("~AI,Teams,Ssl,Error~Attitude: can't call IsFriendly(): 2nd object is NOT valid (object is not entity)");
        return false;
    }

    if (obj1 == obj2)
        return true;

    dsSTRID team1 = GetTeamIDFromEntity(reinterpret_cast<entENTITY *>(obj1));
    dsSTRID team2 = GetTeamIDFromEntity(reinterpret_cast<entENTITY *>(obj2));

    bool neutral12 = gsTEAMS::Instance().GetAttitude(team1, team2) == gsTEAMS::ATT_NEUTRAL;
    bool neutral21 = gsTEAMS::Instance().GetAttitude(team2, team1) == gsTEAMS::ATT_NEUTRAL;
    return neutral12 && neutral21;
}
