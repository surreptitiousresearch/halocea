/* compress_real_to_int16 @0x837E8078 — inverse of uncompress_int16_to_real: quantizes a [-1,1]-ish real to
 * a signed 16-bit sample. floor(z * 32767.5) truncated to int16.
 *
 * DEVIATION: the decompiler's local-variable allocation failed on this function and it emitted confusing
 * bit-reinterpretation casts; rewritten from disasm's actual sequence (fmuls, floor, fctiwz, extsh). */

extern double floor(double x);

__int16 compress_real_to_int16(float z)
{
    return (__int16)(int)floor((double)(z * 32767.5f));
}
