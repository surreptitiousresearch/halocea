/* compress_real_to_int8_clamp @0x837E8030 — clamps z to [0,1] and quantizes to a 0-255 byte.
 *
 * DEVIATION: the decompiler renders the final extraction as `>> 24` (high byte); disasm shows a `clrlwi`
 * that keeps the LOW 8 bits instead. Since the converted value is already in [0,255], `>> 24` would zero
 * it out — reproduced here as a plain byte-range cast matching the disasm. */

unsigned int compress_real_to_int8_clamp(float z)
{
    float clamped = z;

    if (z < 0.0f)
        clamped = 0.0f;
    else if (z > 1.0f)
        clamped = 1.0f;

    return (unsigned char)(int)(clamped * 255.0f);
}
