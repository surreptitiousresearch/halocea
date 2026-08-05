/* oneOverTwoToBitWidthLookupTable @ 0x82257EC0 — .rdata, 17 x float: 1.0f / 2^i for i = 0..16,
 * the dequantization scale table indexed by hkaQuantizeDesc.m_bitWidth in src/havok/hkExpandReal.c.
 * Initializer reconstructed from the binary (big-endian words, every one an exact power of two):
 *   +0x0000 0x3F800000 -> 1.0f          +0x0024 0x3B000000 -> 0.001953125f
 *   +0x0004 0x3F000000 -> 0.5f          +0x0028 0x3A800000 -> 0.0009765625f
 *   +0x0008 0x3E800000 -> 0.25f         +0x002C 0x3A000000 -> 0.00048828125f
 *   +0x000C 0x3E000000 -> 0.125f        +0x0030 0x39800000 -> 0.000244140625f
 *   +0x0010 0x3D800000 -> 0.0625f       +0x0034 0x39000000 -> 0.0001220703125f
 *   +0x0014 0x3D000000 -> 0.03125f      +0x0038 0x38800000 -> 6.103515625e-05f
 *   +0x0018 0x3C800000 -> 0.015625f     +0x003C 0x38000000 -> 3.0517578125e-05f
 *   +0x001C 0x3C000000 -> 0.0078125f    +0x0040 0x37800000 -> 1.52587890625e-05f
 *   +0x0020 0x3B800000 -> 0.00390625f
 *
 * Element count: the ledger extent of 80 is the next-named-symbol gap (nearlyOneQuad_125
 * @0x82257F10). 17 entries occupy 68 bytes and the image holds zero words at +0x44/+0x48/+0x4C;
 * that is alignment up to the 16-byte-aligned hkQuadReal constants this table sits between
 * (nearlyOneQuad_124 / somewhatNearlyNegOneQuad_124 before it, nearlyOneQuad_125 after), not
 * table entries. 0..16 is also the full index range the one and only reader can produce: the
 * binary has exactly one code xref to this object (hkExpandReal @0x83FA11D0), and its generic
 * bit-reader refills 16 bits at a time, so m_bitWidth cannot exceed 16.
 *
 * The declared type in src/havok/hkExpandReal.c (`const float []`) matches; only the bound is
 * added here. */

const float oneOverTwoToBitWidthLookupTable[17] =
{
    1.0f,               /* [ 0] 1 / 2^0  */
    0.5f,               /* [ 1] 1 / 2^1  */
    0.25f,              /* [ 2] 1 / 2^2  */
    0.125f,             /* [ 3] 1 / 2^3  */
    0.0625f,            /* [ 4] 1 / 2^4  */
    0.03125f,           /* [ 5] 1 / 2^5  */
    0.015625f,          /* [ 6] 1 / 2^6  */
    0.0078125f,         /* [ 7] 1 / 2^7  */
    0.00390625f,        /* [ 8] 1 / 2^8  */
    0.001953125f,       /* [ 9] 1 / 2^9  */
    0.0009765625f,      /* [10] 1 / 2^10 */
    0.00048828125f,     /* [11] 1 / 2^11 */
    0.000244140625f,    /* [12] 1 / 2^12 */
    0.0001220703125f,   /* [13] 1 / 2^13 */
    6.103515625e-05f,   /* [14] 1 / 2^14 */
    3.0517578125e-05f,  /* [15] 1 / 2^15 */
    1.52587890625e-05f, /* [16] 1 / 2^16 */
};
