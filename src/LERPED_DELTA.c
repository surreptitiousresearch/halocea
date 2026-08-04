/* LERPED_DELTA @ 0x837ADFD8 — compute a scaled per-component delta between two 3D
 * vectors: dst[i] = (srcB[i] - srcA[i]) * percent. */

void LERPED_DELTA(float *dst, const float *srcA, const float *srcB, float percent)
{
    dst[0] = (srcB[0] - srcA[0]) * percent;
    dst[1] = (srcB[1] - srcA[1]) * percent;
    dst[2] = (srcB[2] - srcA[2]) * percent;
}
