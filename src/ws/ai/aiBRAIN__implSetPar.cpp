#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBODY_IFACE.h"
#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsTSTRING_BUF_HEADER.h"

// ap subsystem logging sink (printf-style). boundary.
extern void _apLog(const char *fmt, ...);

// aiBRAIN::implSetPar(const dsTSTRING<char>&, float) @ 0x83248D38
// ?implSetPar@aiBRAIN@@UAAXABV?$dsTSTRING@D@@M@Z  (virtual)
// SSL op: apply the named tunable `key`=`val`. Offer it to the body first; if the body does not
// consume it, offer it to the mind. Log when nothing recognizes the key.
void aiBRAIN::implSetPar(const dsTSTRING<char> &key, float val)
{
    if (body.pointee->SetPar())
        return; // the body consumed the parameter

    // aiMIND::SetPar reports whether any mind system consumed `key`. DEVIATION: the original source
    // treated the result as a count and had a "recognized by several systems" diagnostic for >1,
    // but that branch is unreachable in this build (SetPar returns a 0/1 bool), so it is omitted.
    if (mind.SetPar(key, val))
        return;

    _apLog("~AI,Error~'%s': key '%s' is not recognized in SetPar() command",
           names.nameInst, key.pBuffer->str);
}
