#include "../../headers/ws/ai/aiCOORDINATOR.h"
#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"

// aiCOORDINATOR::DropAll @ 0x83237908
// ?DropAll@aiCOORDINATOR@@UAAXXZ
//
// Destroy every coordinated group and clear the used-WP result map. Groups are freed via their
// vtable slot-0 scalar-deleting destructor (arg 1 => free storage), then the vector is emptied.
void aiCOORDINATOR::DropAll()
{
    int nElem = groups.nElem;
    for (int i = 0; i < nElem; ++i)
    {
        aiCOORD_GROUP *grp = groups[i];
        if (grp)
            (*reinterpret_cast<void (**)(aiCOORD_GROUP *, int)>(
                *reinterpret_cast<void **>(grp)))(grp, 1);
    }
    groups.nElem = 0;
    resultsUsed.Clear();
}
