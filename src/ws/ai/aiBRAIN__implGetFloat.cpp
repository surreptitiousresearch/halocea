#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsTSTRING_BUF_HEADER.h"

// ap subsystem logging sink (printf-style). boundary.
extern void _apLog(const char *fmt, ...);

// aiBRAIN::implGetFloat(const dsTSTRING<char>&) @ 0x83248DE8
// ?implGetFloat@aiBRAIN@@UAAMABV?$dsTSTRING@D@@@Z  (virtual)
// SSL op: read the float SSL parameter named `desc`. When `desc` is not a known interned parameter
// name, log and return 0.
float aiBRAIN::implGetFloat(const dsTSTRING<char> &desc)
{
    dsSTRID id(desc, true); // existOnly — only match already-interned names
    if (id.IsValid())
        return GetFloatSSL(id);

    _apLog("~AI,Ssl,Error~'%s' GetFloat(\"%s\"): unknown parameter name",
           names.nameInst, desc.pBuffer->str);
    return 0.0f;
}
