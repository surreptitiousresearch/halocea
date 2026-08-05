#pragma once

#include <stdint.h>
#include "hkaAnimation.h"

/* hkaDeltaCompressedAnimation — delta-quantized skeletal animation. All
   per-block source data (quantized stream and the per-DOF offset/scale/bit-width
   tables) lives packed in m_dataBuffer; the *Idx fields are byte offsets into it.
   Layout verified against the DB (types_members: hkaDeltaCompressedAnimation +
   ::QuantizationFormat + _vtbl). */

typedef struct hkQsTransform hkQsTransform;
typedef struct hkaChunkCache hkaChunkCache;
typedef struct hkaAnimation_DataChunk hkaAnimation_DataChunk;           /* hkaAnimation::DataChunk */
typedef struct hkaAnimation_TrackAnnotation hkaAnimation_TrackAnnotation; /* hkaAnimation::TrackAnnotation */

/* types_members hkaDeltaCompressedAnimation::QuantizationFormat (20 bytes) */
typedef struct hkaDeltaCompressedAnimation_QuantizationFormat
{
    unsigned char m_maxBitWidth;   /* +0  */
    unsigned char m_preserved;     /* +1  */
    unsigned char _pad02[2];       /* +2  */
    unsigned int m_numD;           /* +4  */
    unsigned int m_offsetIdx;      /* +8  */
    unsigned int m_scaleIdx;       /* +12 */
    unsigned int m_bitWidthIdx;    /* +16 */
} hkaDeltaCompressedAnimation_QuantizationFormat;

/* Local alias so member spellings match the DB's nested-qualified type. */
typedef hkaDeltaCompressedAnimation_QuantizationFormat QuantizationFormat;

struct hkaDeltaCompressedAnimation : hkaAnimation
{
    int m_numberOfPoses;                       /* +36  */
    int m_blockSize;                           /* +40  */
    QuantizationFormat m_qFormat;              /* +44  */
    unsigned int m_quantizedDataIdx;           /* +64  */
    unsigned int m_quantizedDataSize;          /* +68  */
    unsigned int m_staticMaskIdx;              /* +72  */
    unsigned int m_staticMaskSize;             /* +76  */
    unsigned int m_staticDOFsIdx;              /* +80  */
    unsigned int m_staticDOFsSize;             /* +84  */
    unsigned int m_numStaticTransformDOFs;     /* +88  */
    unsigned int m_numDynamicTransformDOFs;    /* +92  */
    unsigned int m_totalBlockSize;             /* +96  */
    unsigned int m_lastBlockSize;              /* +100 */
    unsigned char *m_dataBuffer;               /* +104 */
    int m_numDataBuffer;                       /* +108 */
};
typedef struct hkaDeltaCompressedAnimation hkaDeltaCompressedAnimation;

/* Concrete vtable — types_members hkaDeltaCompressedAnimation_vtbl (17 slots). */
typedef struct hkaDeltaCompressedAnimation_vtbl
{
    void (*dtr_hkaDeltaCompressedAnimation)(hkaDeltaCompressedAnimation *self);                                                          /* +0  */
    const struct hkClass *(*getClassType)(hkaDeltaCompressedAnimation *self);                                                            /* +4  */
    void (*calcContentStatistics)(hkaDeltaCompressedAnimation *self, struct hkStatisticsCollector *, const struct hkClass *);            /* +8  */
    void (*sampleTracks)(hkaDeltaCompressedAnimation *self, float time, hkQsTransform *out, float *floatsOut, hkaChunkCache *cache);     /* +12 */
    void (*samplePartialTracks)(hkaDeltaCompressedAnimation *self, float time, unsigned int maxTransform, hkQsTransform *out, unsigned int maxFloat, float *floatsOut, hkaChunkCache *cache); /* +16 */
    void (*clearAllCacheKeys)(hkaDeltaCompressedAnimation *self, hkaChunkCache *cache);                                                  /* +20 */
    void (*sampleIndividualTransformTracks)(hkaDeltaCompressedAnimation *self, float time, const int16_t *tracks, unsigned int numTracks, hkQsTransform *out); /* +24 */
    void (*sampleIndividualFloatTracks)(hkaDeltaCompressedAnimation *self, float time, const int16_t *tracks, unsigned int numTracks, float *out); /* +28 */
    int (*getNumOriginalFrames)(hkaDeltaCompressedAnimation *self);                                                                      /* +32 */
    int (*getNumDataChunks)(hkaDeltaCompressedAnimation *self, float time);                                                              /* +36 */
    void (*getDataChunks)(hkaDeltaCompressedAnimation *self, float time, hkaAnimation_DataChunk *chunks, int numChunks);                 /* +40 */
    int (*getMaxSizeOfCombinedDataChunks)(hkaDeltaCompressedAnimation *self);                                                            /* +44 */
    void (*getExtractedMotionReferenceFrame)(hkaDeltaCompressedAnimation *self, float time, hkQsTransform *out);                         /* +48 */
    void (*getExtractedMotionDeltaReferenceFrame)(hkaDeltaCompressedAnimation *self, float t0, float t1, int cropStartAmount, hkQsTransform *out); /* +52 */
    unsigned int (*getNumAnnotations)(hkaDeltaCompressedAnimation *self, float t0, float t1);                                            /* +56 */
    unsigned int (*getAnnotations)(hkaDeltaCompressedAnimation *self, float t0, float t1, hkaAnimation_TrackAnnotation *out, unsigned int maxAnnotations); /* +60 */
    unsigned int (*getFullCacheKey)(hkaDeltaCompressedAnimation *self, unsigned int poseIdx);                                            /* +64 */
} hkaDeltaCompressedAnimation_vtbl;
