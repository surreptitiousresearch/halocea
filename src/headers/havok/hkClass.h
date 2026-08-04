#pragma once
#include "hkFlags.h"

/* Havok runtime reflection descriptor (48 bytes, DB types_members). Compared by
   identity at the call sites; body expanded from the DB. */

typedef struct hkClassEnum hkClassEnum;               /* boundary — reflection enum descriptor */
typedef struct hkClassMember hkClassMember;           /* boundary — reflection member descriptor */
typedef struct hkCustomAttributes hkCustomAttributes; /* boundary — reflection attribute set */

struct hkClass
{
    const char *m_name;                                     /* 0x00 */
    const struct hkClass *m_parent;                         /* 0x04 */
    int m_objectSize;                                       /* 0x08 */
    int m_numImplementedInterfaces;                         /* 0x0C */
    const hkClassEnum *m_declaredEnums;                     /* 0x10 */
    int m_numDeclaredEnums;                                 /* 0x14 */
    const hkClassMember *m_declaredMembers;                 /* 0x18 */
    int m_numDeclaredMembers;                               /* 0x1C */
    const void *m_defaults;                                 /* 0x20 */
    const hkCustomAttributes *m_attributes;                 /* 0x24 */
    enum FlagValues : unsigned int;                         /* nested reflection flag enum (fwd) */
    hkFlags<enum hkClass::FlagValues,unsigned int> m_flags; /* 0x28 */
    int m_describedVersion;                                 /* 0x2C */
};
typedef struct hkClass hkClass;
