#pragma once
#include "hkdAction.h"
#include "hkVector4.h"
#include "hkEnum.h"
#include "hkRefPtr.h"

typedef struct hkdBreakableShape hkdBreakableShape;

/* types_members hkdDecorateFractureFaceAction::FractureFaceInfo size 12 */
typedef struct hkdDecorateFractureFaceAction_FractureFaceInfo
{
    hkRefPtr<const hkdBreakableShape> m_decorations; /* 0  */
    float m_density;                                 /* 4  */
    float m_gap;                                     /* 8  */
} hkdDecorateFractureFaceAction_FractureFaceInfo;

/* Local alias so spellings match the DB's nested-qualified type names. */
typedef hkdDecorateFractureFaceAction_FractureFaceInfo FractureFaceInfo;

/* types_members hkdDecorateFractureFaceAction::FractureFaceInfo2 size 16 —
   FractureFaceInfo base plus a max-slope cutoff (used for the top/bottom faces). */
struct hkdDecorateFractureFaceAction_FractureFaceInfo2 : FractureFaceInfo
{
    float m_maxSlope; /* 12 */
};
typedef struct hkdDecorateFractureFaceAction_FractureFaceInfo2 hkdDecorateFractureFaceAction_FractureFaceInfo2;
typedef hkdDecorateFractureFaceAction_FractureFaceInfo2 FractureFaceInfo2;

/* types_enum_values hkdDecorateFractureFaceAction::DecorationPivot (u8 storage) */
enum DecorationPivot
{
    GEOMETRY_PIVOT = 0,
    AABB_CENTER = 1,
};

/* size 128. Layout from DB types_members hkdDecorateFractureFaceAction. */
struct hkdDecorateFractureFaceAction : hkdAction
{
    unsigned char _pad08[8];                              /* 8   (alignment to 16) */
    hkVector4 m_upDir;                                    /* 16  */
    hkVector4 m_frontDir;                                 /* 32  */
    hkEnum<DecorationPivot, unsigned char> m_decorationPivot; /* 48 */
    unsigned char _pad31[3];                              /* 49  */
    float m_vertGridDist;                                 /* 52  */
    float m_vertGridOffset;                               /* 56  */
    float m_horGridDist;                                  /* 60  */
    float m_horGridOffset;                                /* 64  */
    FractureFaceInfo2 m_top;                              /* 68  */
    FractureFaceInfo2 m_bottom;                           /* 84  */
    FractureFaceInfo m_left;                              /* 100 */
    FractureFaceInfo m_right;                             /* 112 */
};
typedef struct hkdDecorateFractureFaceAction hkdDecorateFractureFaceAction;
