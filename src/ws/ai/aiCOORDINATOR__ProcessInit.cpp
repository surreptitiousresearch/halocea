#include "../../headers/ws/ai/aiCOORDINATOR.h"
#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsSTRID.h"

// aiCOORDINATOR::ProcessInit @ 0x83234208
// ?ProcessInit@aiCOORDINATOR@@UAAXXZ
//
// One-time coordinator init: register each built-in coordinated-group class (name -> class-id).
// The binary unrolls the seven RegisterGroupClass calls, each building a temporary dsTSTRING name
// via UnsafeInit and releasing it afterwards; reproduced faithfully as a table walk (the dsTSTRING
// ctor body is UnsafeInit(s, -1, 0), and its destructor is the inlined refcount-dec / dlFree tail).
void aiCOORDINATOR::ProcessInit()
{
    static const struct GroupClassReg {
        const char    *name;
        const dsSTRID *classId;
    } kGroupClasses[] = {
        { "grp_base",   &aiCOORD_GROUP::GRP_BASE },
        { "grp_squad",  &aiCOORD_GROUP::GRP_SQUAD },
        { "grp_attack", &aiCOORD_GROUP::GRP_ATTACK },
        { "grp_defend", &aiCOORD_GROUP::GRP_DEFEND },
        { "grp_follow", &aiCOORD_GROUP::GRP_FOLLOW },
        { "grp_melee",  &aiCOORD_GROUP::GRP_MELEE },
        { "grp_chase",  &aiCOORD_GROUP::GRP_CHASE },
    };

    const int count = static_cast<int>(sizeof(kGroupClasses) / sizeof(kGroupClasses[0]));
    for (int i = 0; i < count; ++i)
    {
        dsTSTRING<char> name(kGroupClasses[i].name);
        RegisterGroupClass(name, *kGroupClasses[i].classId);
    }
}
