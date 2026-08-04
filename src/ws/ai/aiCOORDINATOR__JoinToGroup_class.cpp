#include "../../headers/ws/ai/aiCOORDINATOR.h"
#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h" // dsSPrintf

// aiCOORDINATOR::JoinToGroup @ 0x83235738  (5-explicit-arg, ssl-class overload)
// ?JoinToGroup@aiCOORDINATOR@@IAA_NPAVaiBRAIN@@ABV?$dsTSTRING@D@@ABVdsSTRID@@ABVsslCLASS_REF@@AAV3@@Z
//
// Try to add `pBrain` to an existing group of type `groupType` (and, unless the requested name is
// empty, matching `groupName`). Returns true on success; on the terminal reject path writes an
// explanatory message into `hint`.
bool aiCOORDINATOR::JoinToGroup(aiBRAIN *pBrain, const dsTSTRING<char> &groupName,
                                const dsSTRID &groupType, const sslCLASS_REF &grpClass,
                                dsTSTRING<char> &hint)
{
    if (!grpClass.pClass)
    {
        hint = dsSPrintf("unknown group type'%s' (group class is null). ", groupType.CStr());
        return false;
    }

    LeaveGroup(pBrain);

    bool anyName = groupName.pBuffer->strLen == 0;
    int nElem = groups.nElem;
    for (int i = 0; i < nElem; ++i)
    {
        aiCOORD_GROUP *grp = groups[i];
        if (groupType.id != grp->GetTypeStrID()->id)
            continue;
        if (anyName || *grp->GetName() == groupName)
        {
            if (groups[i]->AddMember(reinterpret_cast<iaIACTOR *>(pBrain)))
                return true;
            if (!anyName)
            {
                hint = dsSPrintf("group %s:'%s' reject member '%s'. ",
                                 groupType.CStr(), groupName.pBuffer->str, pBrain->names.nameInst);
                return false;
            }
        }
    }
    return false;
}
