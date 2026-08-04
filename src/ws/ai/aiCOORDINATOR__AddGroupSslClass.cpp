#include "../../headers/ws/ai/aiCOORDINATOR.h"

// aiCOORDINATOR::AddGroupSslClass @ 0x83236A20
// ?AddGroupSslClass@aiCOORDINATOR@@IAAXABVsslCLASS_REF@@ABV?$dsTSTRING@D@@ABVdsSTRID@@@Z
//
// Register an ssl group class under a (class-name, type) pair. In debug builds this first
// STRONG_ASSERTs that the (name, type, class) triple is not already present; those checks are
// no-ops in release (IGNORE_STRONG_ASSERT), so only the append survives.
void aiCOORDINATOR::AddGroupSslClass(const sslCLASS_REF &sslGrpClass,
                                     const dsTSTRING<char> &groupClassName,
                                     const dsSTRID &groupType)
{
    aiCOORD_GROUP_SSL_CLASS_DESC desc;
    desc.sslGrpClass = sslGrpClass;
    desc.groupClassName = groupClassName;
    desc.groupType = groupType;
    groupSslClassList.PushBack(desc);
}
