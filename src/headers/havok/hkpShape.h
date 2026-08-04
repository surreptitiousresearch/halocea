#pragma once
#include "hkReferencedObject.h"
#include "hkEnum.h"
#include "hkpShapeType.h"

/* hkpShape base (size 16). Only m_type is read by the reversed callers (shape
   dispatch); m_userData is present for layout. Verified against DB types_members. */
typedef struct hkpShape
{
    hkReferencedObject base;                   /* 0  */
    unsigned int m_userData;                   /* 8  */
#ifdef __cplusplus
    hkEnum<hkpShapeType,unsigned int> m_type;  /* 12 */
#else
    /* hkEnum<ENUM,STORAGE> is a class template, so it cannot be spelled in the C TUs. Its
       layout is exactly the storage scalar (see hkEnum.h), so the C spelling is the storage
       type itself — same size, same offset, no invented layout. */
    unsigned int m_type;                       /* 12 — hkEnum<hkpShapeType,unsigned int> */
#endif
} hkpShape;
