#pragma once
#include "hkMatrix4.h"
#include "hkBool.h"

/* hkdGeometry::ObjectIdentifier, size 80. m_name lands at offset 68 (3 bytes
   of tail padding after the 1-byte m_isInverted). */
typedef struct hkdGeometry_ObjectIdentifier
{
    hkMatrix4 m_transform; /* 0 */
    hkBool m_isInverted;   /* 64 */
    unsigned char _pad0[3]; /* db-verified padding */
    const char *m_name;    /* 68 */
} hkdGeometry_ObjectIdentifier;

/* The DB spells this nested type hkdGeometry::ObjectIdentifier; member declarations
   elsewhere reference the unqualified nested name. */
typedef hkdGeometry_ObjectIdentifier ObjectIdentifier;
