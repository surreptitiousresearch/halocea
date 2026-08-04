#include "../../headers/ws/ai/aiCOORDINATOR.h"
#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiCOORDINATOR::GetGroupTypeName @ 0x83234590
// ?GetGroupTypeName@aiCOORDINATOR@@UAA?AV?$dsTSTRING@D@@PAVaiBRAIN@@@Z
//
// Returns (by value) the type name of `pBrain`'s coordinated group, or "[N/A]" when it has none.
// Verified against disasm: the leading virtual call is GetGroup_2 (vtbl+0x30); when it hits, the
// group's type name is fetched via GetType (vtbl+0x4C) and copied into the returned string.
dsTSTRING<char> aiCOORDINATOR::GetGroupTypeName(aiBRAIN *pBrain)
{
    aiCOORD_GROUP *group = GetGroup_2(pBrain);
    if (group)
        return *group->GetType();

    return dsTSTRING<char>("[N/A]");
}
