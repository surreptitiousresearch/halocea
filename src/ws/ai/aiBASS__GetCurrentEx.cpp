// aiBASS::GetCurrentEx @ 0x83242810
// ?GetCurrentEx@aiBASS@@UAAABV?$dsTSTRING@D@@PAViaIACTOR@@@Z
#include "../../headers/ws/ai/aiBASS.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC.h"   // aiBRAIN_DESC::TYPE_ID
#include "../../headers/ws/ia/iaIACTOR.h"
#include "../../headers/ws/ds/dsTYPE_ID.h"       // IsDerivedFrom

extern void _apLog(const char *fmt, ...); // ?_apLog@@YAXPBDZZ — engine logger  boundary

// Name of the exclamation `actor` is currently speaking. Returns a reference to a process-wide
// scratch string (reset to empty on each call): empty when `actor` is null / not an aiBRAIN, or has
// no live stream entry; otherwise the entry's exclamation id.
const dsTSTRING<char> &aiBASS::GetCurrentEx(iaIACTOR *actor)
{
    static dsTSTRING<char> retVal; // shared scratch result (empty by default)
    retVal = dsTSTRING<char>();

    if (!actor)
    {
        _apLog("~AI,Ssl,Error~'%s' %s(): called with null actor. Only aiBRAIN accepted",
               GetSslObjName().CStr(), "GetCurrentEx");
        return retVal;
    }

    if (!actor->TypeID().IsDerivedFrom(aiBRAIN_DESC::TYPE_ID))
    {
        _apLog("~AI,Ssl,Error~'%s' %s(): '%s' is not aiBRAIN. Only aiBRAIN accepted",
               GetSslObjName().CStr(), "GetCurrentEx", actor->nameIActive.CStr());
        return retVal;
    }

    aiBRAIN *brain = (aiBRAIN *)actor;
    const exSTREAM_DATA *ex = GetExFromStream(brain);
    if (ex)
        retVal = dsTSTRING<char>(ex->name.CStr());

    return retVal;
}
