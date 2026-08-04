#pragma once

/* hkDeltaDecompressionParameters — flat descriptor passed by value to the
   delta block decompressor (_decompress_0). Bundles the quantization format for
   one block plus resolved pointers into the animation's data buffer. Layout
   verified against the DB (types_members: hkDeltaDecompressionParameters). */
typedef struct hkDeltaDecompressionParameters
{
    unsigned int m_numD;                    /* +0  */
    int m_blockSize;                        /* +4  */
    int m_numValues;                        /* +8  poses reconstructed this block */
    unsigned char m_preserved;              /* +12 */
    unsigned char m_maxBitWidth;            /* +13 */
    unsigned char _pad0[2]; /* db-verified padding */
    const float *m_offset;                  /* +16 */
    const float *m_scale;                   /* +20 */
    const unsigned char *m_bitWidth;        /* +24 */
    const unsigned char *m_quantizedData;   /* +28 */
} hkDeltaDecompressionParameters;
