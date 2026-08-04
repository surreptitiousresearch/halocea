#include "../../headers/ws/ai/aiCOORDINATOR.h"

// aiCOORDINATOR::GetGroupSslClassByType @ 0x83235A00
// ?GetGroupSslClassByType@aiCOORDINATOR@@IAA?AVsslCLASS_REF@@ABVdsSTRID@@@Z
//
// The registered ssl group class whose group-type id matches `groupType`, or an empty ref.
sslCLASS_REF aiCOORDINATOR::GetGroupSslClassByType(const dsSTRID &groupType)
{
    int nElem = groupSslClassList.nElem;
    for (int i = 0; i < nElem; ++i)
    {
        aiCOORD_GROUP_SSL_CLASS_DESC &desc = groupSslClassList[i];
        if (groupType.id == desc.groupType.id)
            return desc.sslGrpClass;
    }
    return sslCLASS_REF();
}
