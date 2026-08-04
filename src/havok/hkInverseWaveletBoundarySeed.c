/* hkInverseWaveletBoundarySeed -- inline-only helper reconstructed from the
   VMX128 block inlined at the head of hkInverseWaveletTransform @0x83FA05E8
   (block 0x83FA05FC..0x83FA08B4). No standalone symbol exists in the binary;
   this out-of-line definition stands in for that inlined code so the caller can
   invoke it as a plain function.

   The block does two things:

   Part 1 -- lazy one-time init of the two filter-coefficient row caches
   (topRow / bottomRow), guarded by two bits of the static flag word _S1_333
   (@0x84644910): bit0 -> topRow (0x83FA061C branch), bit1 -> bottomRow
   (0x83FA0710 branch). Each row is built from eight 4-lane rodata coefficient
   vectors and stored into the 32-float cache in vector order:
     topRow[4*i + lane]    = c1(i+1)[lane]   (c11..c18)
     bottomRow[4*i + lane] = c2(i+1)[lane]   (c21..c28)
   Constant addresses (verified from rodata, big-endian floats):
     c11 0x82257E20  c12 0x82257E90  c13 0x82257E80  c14 0x82257E50
     c15 0x82257E70  c16 0x82257E40  c17 0x82257E30  c18 0x82257E60
     c21 0x82257DA0  c22 0x82257E10  c23 0x82257E00  c24 0x82257DD0
     c25 0x82257DF0  c26 0x82257DC0  c27 0x82257DB0  c28 0x82257DE0

   Part 2 -- reconstruct the first eight samples (0x83FA07F4..0x83FA08B4). The
   eight inputs data[0..7] are broadcast (vspltw) and combined by a vmaddfp
   ladder using coefficient vectors c12..c18 (top) and c22..c28 (bottom); c11 /
   c21 (all ones) are cached but not used by the seed. Each of the four VMX lanes
   carries a distinct coefficient, so lane L of a result is output sample L. The
   two results are stored as data[0..3] (top) and data[4..7] (bottom).

   DEVIATION 1: AltiVec vmaddfp vD,vA,vB,vC computes vA*vC + vB (the addend is the
   2nd listed source), so the ladder is a per-lane DOT PRODUCT, not a Horner
   evaluation:
     data[L]   = d0 + c12[L]*d1 + c13[L]*d2 + ... + c18[L]*d7
     data[4+L] = d0 + c22[L]*d1 + c23[L]*d2 + ... + c28[L]*d7
   d0 enters as the initial addend (weight 1); c18/c25 lanes that are zero simply
   drop their term. This is a fixed 8x8 boundary reconstruction matrix.

   DEVIATION 2: the 4-wide VMX FMA is scalarised into a per-lane loop. The FMA
   accumulation order is preserved, but a fused vmaddfp rounds once whereas the C
   `acc += c*d` rounds the product and the add separately; results are identical
   for these exact power-of-two coefficients. */

/* boundary -- filter-coefficient row caches (BSS globals, shared with
   hkInverseWaveletTransform's lifting steps hkUpdateReflect / hkPredictReflect). */
extern float topRow[32];
extern float bottomRow[32];

/* Compile-time coefficient vectors, index [i] = c(row)(i+1), lane order [0..3].
   Values read verbatim from rodata (see header comment for addresses). */
static const float hkWaveletTopCoeffs[8][4] = {
    { 1.0f,   1.0f,     1.0f,    1.0f    },  /* c11 (cached, unused by seed) */
    { -0.5f,  -0.25f,   0.0f,    0.25f   },  /* c12 */
    { -0.5f,  0.0625f,  0.625f,  0.1875f },  /* c13 */
    { 0.0f,   -0.0625f, -0.125f, -0.1875f }, /* c14 */
    { -0.5f,  0.625f,   -0.25f,  -0.125f },  /* c15 */
    { 0.0f,   -0.125f,  -0.25f,  0.75f   },  /* c16 */
    { 0.0f,   0.0f,     0.0f,    -0.125f },  /* c17 */
    { 0.0f,   0.0f,     0.0f,    0.0f    }   /* c18 */
};

static const float hkWaveletBottomCoeffs[8][4] = {
    { 1.0f,   1.0f,    1.0f,    1.0f    },  /* c21 (cached, unused by seed) */
    { 0.5f,   0.5f,    0.5f,    0.5f    },  /* c22 */
    { -0.25f, -0.25f,  -0.25f,  -0.25f  },  /* c23 */
    { -0.25f, 0.25f,   0.75f,   0.75f   },  /* c24 */
    { 0.0f,   0.0f,    0.0f,    0.0f    },  /* c25 */
    { -0.25f, -0.125f, 0.0f,    0.0f    },  /* c26 */
    { -0.25f, 0.75f,   -0.25f,  -0.25f  },  /* c27 */
    { 0.0f,   -0.125f, -0.25f,  0.75f   }   /* c28 */
};

/* Lazy-init guard word (binary: static _S1_333). bit0 = topRow, bit1 = bottomRow. */
static unsigned int hkWaveletBoundaryCoeffInit = 0;

void hkInverseWaveletBoundarySeed(float *data)
{
    int row;
    int lane;
    float sample[8];

    /* Part 1: build the coefficient row caches on first use. */
    if ((hkWaveletBoundaryCoeffInit & 1) == 0)
    {
        for (row = 0; row < 8; row++)
            for (lane = 0; lane < 4; lane++)
                topRow[row * 4 + lane] = hkWaveletTopCoeffs[row][lane];
        hkWaveletBoundaryCoeffInit |= 1;
    }
    if ((hkWaveletBoundaryCoeffInit & 2) == 0)
    {
        for (row = 0; row < 8; row++)
            for (lane = 0; lane < 4; lane++)
                bottomRow[row * 4 + lane] = hkWaveletBottomCoeffs[row][lane];
        hkWaveletBoundaryCoeffInit |= 2;
    }

    /* Part 2: capture the first eight samples before any store (the VMX code
       loads all of data[0..7] before writing data[0..7]). */
    for (row = 0; row < 8; row++)
        sample[row] = data[row];

    /* Reconstruct data[0..3] (top) and data[4..7] (bottom). Lane L -> sample L.
       d0 is the initial addend (weight 1); the remaining terms use c*(k+1). */
    for (lane = 0; lane < 4; lane++)
    {
        float top = sample[0];
        float bottom = sample[0];
        int k;

        for (k = 1; k < 8; k++)
        {
            top += hkWaveletTopCoeffs[k][lane] * sample[k];
            bottom += hkWaveletBottomCoeffs[k][lane] * sample[k];
        }

        data[lane] = top;
        data[4 + lane] = bottom;
    }
}
