#include "../../headers/ws/ai/aiCOORDINATOR.h"
#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h" // dsSPrintf
#include "../../headers/ws/ds/ds_new.h"           // tracked operator new(size_t, const char*, int)

// aiCOORDINATOR::CreateGroup @ 0x832364D0
// ?CreateGroup@aiCOORDINATOR@@UAA_NPAVaiBRAIN@@V?$dsTSTRING@D@@ABVdsSTRID@@_NAAV3@@Z
//
// Create a new coordinated group of type `groupType` with `pBrain` as its first member. When
// `canJoinToExist` is set, an existing matching group is joined instead. `groupName` is taken by
// value (it may be rewritten to an auto-generated "Auto_<type>_<id>" name when empty). Diagnostics
// go into `hint`. Returns true on success.
bool aiCOORDINATOR::CreateGroup(aiBRAIN *pBrain, dsTSTRING<char> groupName,
                                const dsSTRID &groupType, bool canJoinToExist,
                                dsTSTRING<char> &hint)
{
    sslCLASS_REF grpClass = GetGroupSslClassByType(groupType);
    if (!grpClass.pClass)
    {
        hint = dsSPrintf("unknown group type: %s (can't find group class by type while create group)",
                         groupType.CStr());
        return false;
    }

    if (!pBrain->GetNavSystem())
    {
        hint = dsSPrintf("ai unit '%s' without nav system", pBrain->names.nameInst);
        return false;
    }

    if (!canJoinToExist || !JoinToGroup(pBrain, groupName, groupType, grpClass, hint))
    {
        LeaveGroup(pBrain);

        if (groupName.pBuffer->strLen && GetGroup_3(&groupName))
        {
            if (canJoinToExist)
            {
                hint.Insert(hint.pBuffer->strLen, "failed to join existing group", -1);
                return false;
            }
            _apLog("~AI,Error~'%s': CreateGroup() fail: Can't create group named '%s'; "
                   "group is already exist!",
                   pBrain->names.nameInst, groupName.pBuffer->str);
            hint += dsSPrintf("join group is disabled and group '%s' is already exist",
                              groupName.pBuffer->str);
            return false;
        }

        if (!groupName.pBuffer->strLen)
            groupName = dsSPrintf("Auto_%s_%d", groupType.CStr(), groupNextID);

        aiCOORD_GROUP *grp = new (
            "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_coord.cpp", 264)
            aiCOORD_GROUP(&groupName, &groupType, &grpClass);

        int nextId = groupNextID + 1;
        groupNextID = nextId;
        if (nextId < 0)
            groupNextID = 0;

        if (!grp->AddMember(reinterpret_cast<iaIACTOR *>(pBrain)))
        {
            // vtable slot 0: scalar-deleting destructor (arg 1 => free the object).
            (*reinterpret_cast<void (**)(aiCOORD_GROUP *, int)>(
                *reinterpret_cast<void **>(grp)))(grp, 1);
            hint = dsSPrintf("just created group %s:'%s' reject first member (creator) '%s'",
                             groupType.CStr(), groupName.pBuffer->str, pBrain->names.nameInst);
            return false;
        }
        groups.PushBack(grp);
    }
    return true;
}
