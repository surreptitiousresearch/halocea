#include "../../headers/ws/ai/aiBASS.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC.h"
#include "../../headers/ws/ai/aiBRAIN_boundary.h" // _apLog
#include "../../headers/ws/ia/iaIACTOR.h"

// aiBASS::PlayExclamationInstant @ 0x83241140
// ?PlayExclamationInstant@aiBASS@@UAA_NPAViaIACTOR@@ABV?$dsTSTRING@D@@@Z
//
// Play `sound` immediately (synchronised) on `actor`, which must be an aiBRAIN. Non-brain or null
// actors are rejected with a diagnostic.
bool aiBASS::PlayExclamationInstant(iaIACTOR *actor, const dsTSTRING<char> &sound)
{
    if (!actor)
    {
        _apLog("~AI,Ssl,Error~'%s' %s(): called with null actor. Only aiBRAIN accepted",
               GetSslObjName().pBuffer->str, "PlayExclamationInstant");
        return false;
    }

    if (!actor->TypeID().IsDerivedFrom(aiBRAIN_DESC::TYPE_ID))
    {
        // DEVIATION: the original also logs the actor's descriptor type-name, fetched via an
        // untyped vtable-slot access (id.array[0]+48, +12); it is diagnostic-only and omitted here.
        _apLog("~AI,Ssl,Error~'%s' %s(): '%s' is not aiBRAIN. Only aiBRAIN accepted",
               GetSslObjName().pBuffer->str, "PlayExclamationInstant", "");
        return false;
    }

    dsSTRID snd(sound, false);
    actor->PlaySoundSynced(snd, 0);
    return true;
}
