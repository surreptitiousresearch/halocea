#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsTSTRING_BUF_HEADER.h"

// ap subsystem logging sink (printf-style). boundary.
extern void _apLog(const char *fmt, ...);

// aiBRAIN::implSetFloat(const dsTSTRING<char>&, float) @ 0x83248E70
// ?implSetFloat@aiBRAIN@@UAAXABV?$dsTSTRING@D@@M@Z  (virtual)
// SSL op: set the float SSL parameter named `desc` to `value`. No writable float SSL parameters
// exist, so every call is rejected with an "unknown parameter" diagnostic. The interned id is
// still constructed (as the original does) though its result is unused here.
void aiBRAIN::implSetFloat(const dsTSTRING<char> &desc, float value)
{
    dsSTRID id(desc, true);
    (void)id;
    _apLog("~AI,Ssl,Error~'%s' SetFloat(\"%s\", %f): unknown parameter name",
           names.nameInst, desc.pBuffer->str, value);
}
