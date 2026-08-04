#include "../../headers/ws/ai/aiCOORDINATOR.h"
#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiCOORDINATOR::GetGroupName @ 0x832344E0
// ?GetGroupName@aiCOORDINATOR@@UAAABV?$dsTSTRING@D@@PAVaiBRAIN@@@Z
//
// The name of `pBrain`'s coordinated group, or a shared "[no group]" fallback when it has none.
// The fallback is a lazily-initialised static (the binary's guarded one-shot init + atexit dtor).
const dsTSTRING<char> &aiCOORDINATOR::GetGroupName(aiBRAIN *pBrain)
{
    aiCOORD_GROUP *group = GetGroup_2(pBrain);
    if (group)
        return *group->GetName();

    static dsTSTRING<char> noGroupName("[no group]");
    return noGroupName;
}
