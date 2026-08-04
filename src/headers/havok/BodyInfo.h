#pragma once
#include "hkVector4.h"
#include "hkTransform.h"
#include "hkInplaceArrayAligned16.h"

typedef struct hkdShapeInstanceInfo hkdShapeInstanceInfo;
typedef struct hkdGraphicsShape hkdGraphicsShape;
typedef struct hkdBreakableBody hkdBreakableBody;

/* BodyInfo::Normal — a used separating plane (packed as an hkVector4: xyz normal,
   w plane offset) tagged with the shape instance it came from. DB reports the
   live fields at 0/16; the inline-array stride is 32 (16-byte aligned), so the
   struct is padded to 32 for correct indexing. */
typedef struct BodyInfo_Normal
{
    hkVector4 m_normal;                       /* 0  plane { nx, ny, nz, d } */
    const hkdShapeInstanceInfo *m_shapeInstance; /* 16 */
    unsigned char _pad20[12];                 /* 20 -> 32 (array-stride padding) */
} BodyInfo_Normal;

/* BodyInfo::Graphics — a placed decoration graphics shape plus its world
   transform. Live fields at 0/64; padded to the 80-byte inline-array stride. */
typedef struct BodyInfo_Graphics
{
    hkTransform m_transform;             /* 0  */
    const hkdGraphicsShape *m_graphics;  /* 64 */
    unsigned char _pad44[12];            /* 68 -> 80 (array-stride padding) */
} BodyInfo_Graphics;

/* Local aliases so member spellings match the DB's nested-qualified template
   arguments (hkInplaceArrayAligned16<BodyInfo::Normal,12> etc.). */
typedef BodyInfo_Normal Normal;
typedef BodyInfo_Graphics Graphics;

/* BodyInfo, size 1392 — per-breakable-body decoration accumulator. */
typedef struct BodyInfo
{
    hkdBreakableBody *m_body;                             /* 0    */
    unsigned char _pad04[12];                             /* 4    (align to 16) */
    hkInplaceArrayAligned16<Normal, 12> m_usedNormals;    /* 16   (400 bytes) */
    hkInplaceArrayAligned16<Graphics, 12> m_graphics;     /* 416  (976 bytes) */
} BodyInfo;                                               /* 1392 bytes */
