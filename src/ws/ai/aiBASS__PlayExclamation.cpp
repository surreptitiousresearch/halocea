#include "../../headers/ws/ai/aiBASS.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC.h"
#include "../../headers/ws/ai/aiBRAIN_boundary.h" // _apLog
#include "../../headers/ws/ia/iaIACTOR.h"

// aiBASS::PlayExclamation @ 0x832412C8
// ?PlayExclamation@aiBASS@@UAA_NPAViaIACTOR@@ABV?$dsTSTRING@D@@M1@Z
//
// Queue an exclamation `sound` for `actor` (which must be an aiBRAIN), optionally overriding the
// configured priority (>= 0) and overlap policy, then hand it to InternalPlayEx.
bool aiBASS::PlayExclamation(iaIACTOR *actor, const dsTSTRING<char> &sound, float priority,
                             const dsTSTRING<char> &overlap)
{
    if (!actor)
    {
        _apLog("~AI,Ssl,Error~'%s' %s(): called with null actor. Only aiBRAIN accepted",
               GetSslObjName().pBuffer->str, "PlayExclamation");
        return false;
    }

    if (!actor->TypeID().IsDerivedFrom(aiBRAIN_DESC::TYPE_ID))
    {
        // DEVIATION: original also logs the actor's descriptor type-name (untyped vtable-slot
        // access, diagnostic-only) — omitted here.
        _apLog("~AI,Ssl,Error~'%s' %s(): '%s' is not aiBRAIN. Only aiBRAIN accepted",
               GetSslObjName().pBuffer->str, "PlayExclamation", "");
        return false;
    }

    // DEVIATION: the "Adding exclamation by %s" line logs the actor's descriptor type-name via the
    // same untyped access; passed empty here.
    _apLog("~BASS~==>Adding exclamation by %s sound '%s'", "", sound.pBuffer->str);

    dsSTRID sndId(sound, true);
    if (!sndId.IsValid())
    {
        _apLog("~BASS,Error~: PlayExclamation(): invalid sound '%s'", sound.pBuffer->str);
        return false;
    }

    exDATA ex = FindExclamation(sndId);
    if (!ex.name.IsValid())
    {
        _apLog("~BASS,Error~: PlayExclamation(): sound '%s' not defined", sound.pBuffer->str);
        return false;
    }

    if (priority >= 0.0f)
        ex.priority = priority;

    exOVERLAP_RES ov = ParseOverlap(&overlap, exOVERLAP_INVALID);
    if (ov != exOVERLAP_INVALID)
        ex.overlap = ov;

    return InternalPlayEx((aiBRAIN *)actor, ex);
}
