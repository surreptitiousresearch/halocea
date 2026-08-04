/* compress_real_to_int16_clamp @0x837E80B8 — decompiler flagged "local variable allocation has failed"
 * and rendered the fctiwz-based float-to-int conversion as a broken 64-bit-store/32-bit-reload sequence.
 * disasm_range(0x837E80B8,0x837E8118) shows: clamp z to [-1,1], scale by 32767.5, floor(), convert to a
 * 32-bit int via fctiwz, then sign-extend only the low 16 bits into the __int16 return. */

extern double floor(double x);

__int16 compress_real_to_int16_clamp(float z)
{
    float clamped = z;

    if ( z >= -1.0f )
    {
        clamped = 1.0f;
        if ( z <= 1.0f )
            clamped = z;
    }

    int scaled = (int)floor(clamped * 32767.5f);
    return (__int16)scaled;
}
