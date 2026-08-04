#pragma once
#include "hkReferencedObject.h"
#include "hkEnum.h"

/* hkaAnimation — Havok SDK animation base class (referenced-object header,
   duration, track/annotation arrays). Base subobject of
   hkaDeltaCompressedAnimation / hkaWaveletCompressedAnimation.
   Layout from types_members hkaAnimation (36 bytes). */

typedef struct hkaAnimatedReferenceFrame hkaAnimatedReferenceFrame;
typedef struct hkaAnnotationTrack hkaAnnotationTrack;

/* types_enum_values hkaAnimation::AnimationType */
enum AnimationType
{
    HK_UNKNOWN_ANIMATION = 0,
    HK_INTERLEAVED_ANIMATION = 1,
    HK_DELTA_COMPRESSED_ANIMATION = 2,
    HK_WAVELET_COMPRESSED_ANIMATION = 3,
    HK_MIRRORED_ANIMATION = 4,
    HK_SPLINE_COMPRESSED_ANIMATION = 5,
};

struct hkaAnimation : hkReferencedObject
{
    hkEnum<AnimationType, int> m_type;                     /* 0x08 */
    float m_duration;                                      /* 0x0C */
    int m_numberOfTransformTracks;                         /* 0x10 */
    int m_numberOfFloatTracks;                             /* 0x14 */
    const hkaAnimatedReferenceFrame *m_extractedMotion;    /* 0x18 */
    hkaAnnotationTrack **m_annotationTracks;               /* 0x1C */
    int m_numAnnotationTracks;                             /* 0x20 */
};
typedef struct hkaAnimation hkaAnimation;
