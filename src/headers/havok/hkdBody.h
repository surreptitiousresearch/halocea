#pragma once
#include "hkReferencedObject.h"
#include "hkRefPtr.h"
#include "hkArray.h"
#include "hkBool.h"
#include "hkdController.h"

/* hkdBody (size 36) — destruction body resource record. DB-verified layout
   (types_members hkdBody). */

typedef struct hkxAttribute hkxAttribute; /* boundary — only referenced through hkArray */

struct hkdBody : hkReferencedObject
{
    const char             *m_parentName;           /* 8  */
    hkRefPtr<hkdController> m_controller;           /* 12 */
    hkBool                  m_attachToNearbyObjects;/* 16 */
    unsigned char           _pad11[3];              /* 17 (alignment) */
    float                   m_constraintStrength;   /* 20 */
    hkArray<hkxAttribute>   m_attributes;           /* 24 */
};
typedef struct hkdBody hkdBody;
