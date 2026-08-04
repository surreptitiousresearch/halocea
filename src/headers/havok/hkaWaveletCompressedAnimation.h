#pragma once

#include "hkaAnimation.h"

/* hkaWaveletCompressedAnimation — wavelet-quantized skeletal animation.
   All per-block source data (block index table, quantized stream, and the
   per-DOF offset/scale/bit-width tables) lives packed in m_dataBuffer; the
   *Idx fields are byte offsets into it. Layout verified against the DB
   (types_members: hkaWaveletCompressedAnimation + ::QuantizationFormat + _vtbl). */

typedef struct hkQsTransform hkQsTransform;
typedef struct hkaChunkCache hkaChunkCache;
typedef struct hkaAnimation_DataChunk hkaAnimation_DataChunk;             /* hkaAnimation::DataChunk */
typedef struct hkaAnimation_TrackAnnotation hkaAnimation_TrackAnnotation; /* hkaAnimation::TrackAnnotation */

/* types_members hkaWaveletCompressedAnimation::QuantizationFormat (20 bytes) */
typedef struct hkaWaveletCompressedAnimation_QuantizationFormat
{
    unsigned char m_maxBitWidth;   /* +0  */
    unsigned char m_preserved;     /* +1  */
    unsigned char _pad02[2];       /* +2  */
    unsigned int m_numD;           /* +4  */
    unsigned int m_offsetIdx;      /* +8  */
    unsigned int m_scaleIdx;       /* +12 */
    unsigned int m_bitWidthIdx;    /* +16 */
} hkaWaveletCompressedAnimation_QuantizationFormat;

/* Local alias so member spellings match the DB's nested-qualified type. */
typedef hkaWaveletCompressedAnimation_QuantizationFormat QuantizationFormat;

struct hkaWaveletCompressedAnimation : hkaAnimation
{
    int m_numberOfPoses;                   /* +36  */
    int m_blockSize;                       /* +40  */
    QuantizationFormat m_qFormat;          /* +44  */
    unsigned int m_staticMaskIdx;          /* +64  */
    unsigned int m_staticDOFsIdx;          /* +68  */
    unsigned int m_numStaticTransformDOFs; /* +72  */
    unsigned int m_numDynamicTransformDOFs;/* +76  */
    unsigned int m_blockIndexIdx;          /* +80  */
    unsigned int m_blockIndexSize;         /* +84  */
    unsigned int m_quantizedDataIdx;       /* +88  */
    unsigned int m_quantizedDataSize;      /* +92  */
    unsigned char *m_dataBuffer;           /* +96  */
    int m_numDataBuffer;                   /* +100 */
};
typedef struct hkaWaveletCompressedAnimation hkaWaveletCompressedAnimation;

/* Concrete vtable — types_members hkaWaveletCompressedAnimation_vtbl (17 slots). */
typedef struct hkaWaveletCompressedAnimation_vtbl
{
    void (*dtr_hkaWaveletCompressedAnimation)(hkaWaveletCompressedAnimation *self);                                                        /* +0  */
    const struct hkClass *(*getClassType)(hkaWaveletCompressedAnimation *self);                                                            /* +4  */
    void (*calcContentStatistics)(hkaWaveletCompressedAnimation *self, struct hkStatisticsCollector *, const struct hkClass *);            /* +8  */
    void (*sampleTracks)(hkaWaveletCompressedAnimation *self, float time, hkQsTransform *out, float *floatsOut, hkaChunkCache *cache);     /* +12 */
    void (*samplePartialTracks)(hkaWaveletCompressedAnimation *self, float time, unsigned int maxTransform, hkQsTransform *out, unsigned int maxFloat, float *floatsOut, hkaChunkCache *cache); /* +16 */
    void (*clearAllCacheKeys)(hkaWaveletCompressedAnimation *self, hkaChunkCache *cache);                                                  /* +20 */
    void (*sampleIndividualTransformTracks)(hkaWaveletCompressedAnimation *self, float time, const __int16 *tracks, unsigned int numTracks, hkQsTransform *out); /* +24 */
    void (*sampleIndividualFloatTracks)(hkaWaveletCompressedAnimation *self, float time, const __int16 *tracks, unsigned int numTracks, float *out); /* +28 */
    int (*getNumOriginalFrames)(hkaWaveletCompressedAnimation *self);                                                                      /* +32 */
    int (*getNumDataChunks)(hkaWaveletCompressedAnimation *self, float time);                                                              /* +36 */
    void (*getDataChunks)(hkaWaveletCompressedAnimation *self, float time, hkaAnimation_DataChunk *chunks, int numChunks);                 /* +40 */
    int (*getMaxSizeOfCombinedDataChunks)(hkaWaveletCompressedAnimation *self);                                                            /* +44 */
    void (*getExtractedMotionReferenceFrame)(hkaWaveletCompressedAnimation *self, float time, hkQsTransform *out);                         /* +48 */
    void (*getExtractedMotionDeltaReferenceFrame)(hkaWaveletCompressedAnimation *self, float t0, float t1, int cropStartAmount, hkQsTransform *out); /* +52 */
    unsigned int (*getNumAnnotations)(hkaWaveletCompressedAnimation *self, float t0, float t1);                                            /* +56 */
    unsigned int (*getAnnotations)(hkaWaveletCompressedAnimation *self, float t0, float t1, hkaAnimation_TrackAnnotation *out, unsigned int maxAnnotations); /* +60 */
    unsigned int (*getFullCacheKey)(hkaWaveletCompressedAnimation *self, unsigned int poseIdx);                                            /* +64 */
} hkaWaveletCompressedAnimation_vtbl;
