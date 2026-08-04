#pragma once

/* hkWaveletDecompressionParameters — flat descriptor passed by value to the
   wavelet block decompressor. Bundles the quantization format for one block
   plus resolved pointers into the animation's data buffer. Layout verified
   against the DB (types_members: hkWaveletDecompressionParameters). */
typedef struct hkWaveletDecompressionParameters
{
    unsigned int m_numD;                    /* +0  */
    int m_blockSize;                        /* +4  */
    unsigned char m_preserved;              /* +8  */
    unsigned char m_maxBitWidth;            /* +9  */
    unsigned char _pad0[2]; /* db-verified padding */
    const float *m_offset;                  /* +12 */
    const float *m_scale;                   /* +16 */
    const unsigned char *m_bitWidth;        /* +20 */
    const unsigned char *m_encodedData;     /* +24 */
} hkWaveletDecompressionParameters;
