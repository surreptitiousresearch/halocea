#include "../../headers/ws/ai/aiCOORDINATOR.h"
#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiCOORDINATOR::GetGroup (by name) @ 0x83235090
// ?GetGroup@aiCOORDINATOR@@UAAPAVaiCOORD_GROUP@@ABV?$dsTSTRING@D@@@Z
//
// Linear lookup of a registered group by its name; null when none matches. (The binary inlines the
// dsTSTRING comparison — same-buffer fast path, then length + byte compare — folded here to the
// dsTSTRING equality operator.)
aiCOORD_GROUP *aiCOORDINATOR::GetGroup_3(const dsTSTRING<char> *name)
{
    int nElem = this->groups.nElem;
    for (int i = 0; i < nElem; ++i)
    {
        aiCOORD_GROUP *group = this->groups[i];
        if (*group->GetName() == *name)
            return this->groups[i];
    }
    return nullptr;
}
