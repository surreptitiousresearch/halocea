#include "../../headers/ws/ai/aiCOORDINATOR.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h" // dsSPrintf

// aiCOORDINATOR::JoinToGroup @ 0x83235CD8  (public virtual, resolves the group class by type)
// ?JoinToGroup@aiCOORDINATOR@@UAA_NPAVaiBRAIN@@ABV?$dsTSTRING@D@@ABVdsSTRID@@AAV3@@Z
//
// Look up the ssl group class for `groupType`, then delegate to the class-taking overload. If no
// class is registered for the type, report it through `hint` and fail.
bool aiCOORDINATOR::JoinToGroup(aiBRAIN *pBrain, const dsTSTRING<char> &groupName,
                               const dsSTRID &groupType, dsTSTRING<char> &hint)
{
    sslCLASS_REF grpClass = GetGroupSslClassByType(groupType);
    if (grpClass.pClass)
        return JoinToGroup(pBrain, groupName, groupType, grpClass, hint);

    hint = dsSPrintf("unknown group type: %s (can't find group class by type while join group). ",
                     groupType.CStr());
    return false;
}
