/* hkInverseWaveletTransform @0x83FA05E8 */
/* hkInverseWaveletTransform — reconstruct a signal from its lifting-scheme
   wavelet coefficients, in place.

   The algorithm has three parts:
     1. Lazy one-time initialisation of the two filter-coefficient row caches
        (topRow / bottomRow), guarded by two bits of a static flag word. On first
        use each row is built from the compile-time coefficient constants and
        splatted into the 128-bit lane layout the boundary seed expects.
     2. A boundary "seed" step that reconstructs the first 8 samples with a fused
        multiply-add ladder over the cached coefficient rows (VMX128 vmaddfp).
     3. The doubling refinement loop: for each block size (starting at 8 and
        doubling) apply the inverse update/predict lifting steps and interleave
        the odd/even halves, until the whole nValues-length signal is covered.

   DEVIATION: parts 1 and 2 are 32-wide VMX128 code (lvx128 / stvx128 / vmaddfp
   over registers seeded from the coefficient constants c11..c18 / c21..c28).
   They operate solely on the coefficient caches and the first 8 samples and are
   impractical to scalarise numerically without the coefficient constant values;
   they are represented here by the boundary helper hkInverseWaveletBoundarySeed,
   which stands in for that inlined block. The doubling loop (part 3) is the real
   algorithm and is reproduced faithfully. */

/* boundary — filter-coefficient row caches and their init flag (_S1_333). */
extern float topRow[32];
extern float bottomRow[32];

/* boundary lifting steps (level-1 callees). */
extern void hkUpdateReflect(float weight, float *filter, float *data, const float *dataEnd);   /* hkUpdateReflect */
extern void hkPredictReflect(float weight, const float *filter, float *data, const float *dataEnd); /* hkPredictReflect */
extern void hkMergeOddEven(float *dst, const float *src, unsigned int n);                       /* hkMergeOddEven */

/* boundary — inlined VMX coefficient lazy-init + first-8-sample seed reconstruction.
   Builds/uses topRow & bottomRow and writes the reconstructed data[0..7]. */
extern void hkInverseWaveletBoundarySeed(float *data);

void hkInverseWaveletTransform(float *data, unsigned int nValues)
{
    unsigned int blockSize;

    /* Parts 1 + 2: coefficient caches + boundary 8-sample seed (see header note). */
    hkInverseWaveletBoundarySeed(data);

    /* Part 3: doubling refinement. */
    if (nValues > 8)
    {
        blockSize = 8;
        do
        {
            hkUpdateReflect(-0.25f, topRow, data, &data[blockSize]);
            hkPredictReflect(0.5f, bottomRow, data, &data[blockSize]);
            blockSize *= 2;
            hkMergeOddEven(data, data, blockSize);
        }
        while (blockSize < nValues);
    }
}
