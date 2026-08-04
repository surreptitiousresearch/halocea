#include "../../headers/ws/ai/aiCOORDINATOR.h"
#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"

// aiCOORDINATOR::RemoveDeletedGroups @ 0x83236038
// ?RemoveDeletedGroups@aiCOORDINATOR@@IAAXXZ
//
// Drop any group whose backing ssl object is no longer valid: notify peers, destroy the group,
// and erase it from the vector. Iterates back-to-front so Erase does not shift unvisited entries.
void aiCOORDINATOR::RemoveDeletedGroups()
{
    for (int i = groups.nElem - 1; i >= 0; --i)
    {
        aiCOORD_GROUP *grp = groups[i];
        if (!grp->IsValidSslObj())
        {
            NoticeRemoveOtherGroup(groups[i]);
            aiCOORD_GROUP *dead = groups[i];
            if (dead)
            {
                // vtable slot 0: scalar-deleting destructor (arg 1 => free the object).
                (*reinterpret_cast<void (**)(aiCOORD_GROUP *, int)>(
                    *reinterpret_cast<void **>(dead)))(dead, 1);
            }
            groups.Erase(i, 1);
        }
    }
}
