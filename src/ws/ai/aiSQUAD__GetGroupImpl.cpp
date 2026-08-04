#include "../../headers/ws/ai/aiSQUAD.h"
#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsTSTRING.h"

extern void _apLog(const char *fmt, ...); // engine formatted log sink. boundary.

// aiSQUAD::GetGroupImpl() @ 0x832A8E18  (?GetGroupImpl@aiSQUAD@@MAAPAVaiCOORD_GROUP@@XZ, protected)
// Resolve this squad's coordinated group: fetch the SSL "GetGroup" result and return its callback
// host as an aiCOORD_GROUP*. When the squad is empty (no group object), log an error and return null.
aiCOORD_GROUP *aiSQUAD::GetGroupImpl()
{
    sslOBJ_REF group = GetGroup();
    if (group.pObject)
        return reinterpret_cast<aiCOORD_GROUP *>(group.GetCbHost());

    dsTSTRING<char> name = GetName();
    _apLog("~AI,Ssl,Error~Call squad function on empty squad '%s'.", name.pBuffer->str);
    return nullptr;
    // name's destructor releases its buffer reference at scope end.
}
