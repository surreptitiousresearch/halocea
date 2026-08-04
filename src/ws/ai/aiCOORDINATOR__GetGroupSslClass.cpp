#include "../../headers/ws/ai/aiCOORDINATOR.h"

// aiCOORDINATOR::GetGroupSslClass @ 0x83235908
// ?GetGroupSslClass@aiCOORDINATOR@@IAA?AVsslCLASS_REF@@ABV?$dsTSTRING@D@@@Z
//
// The registered ssl group class whose class-name matches `groupClassName`, or an empty ref.
sslCLASS_REF aiCOORDINATOR::GetGroupSslClass(const dsTSTRING<char> &groupClassName)
{
    int nElem = groupSslClassList.nElem;
    for (int i = 0; i < nElem; ++i)
    {
        aiCOORD_GROUP_SSL_CLASS_DESC &desc = groupSslClassList[i];
        if (desc.groupClassName == groupClassName)
            return desc.sslGrpClass;
    }
    return sslCLASS_REF();
}
