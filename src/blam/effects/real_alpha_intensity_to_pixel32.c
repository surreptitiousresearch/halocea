/* real_alpha_intensity_to_pixel32 @0x8379E038 — packs an alpha and a grayscale intensity into an ARGB32
 * pixel (R=G=B=intensity).
 *
 * DEVIATION: the decompiler renders the byte-packing as a chain of `__ROL4__` bit tricks; disasm shows a
 * plain shift/or sequence, reproduced here directly. */

unsigned int real_alpha_intensity_to_pixel32(float alpha, float intensity)
{
    unsigned int a = (unsigned char)(int)(alpha * 255.0f);
    unsigned int i = (unsigned char)(int)(intensity * 255.0f);

    return (a << 24) | (i << 16) | (i << 8) | i;
}
