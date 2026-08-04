/* DEVIATION: decompiler shows >> 24 (wrong); disasm: fctiwz + lwz at back_chain+4 + clrlwi(24) = & 0xFF */
unsigned int compress_real_to_int8(float z)
{
    return (unsigned int)(unsigned char)(int)(z * 255.0f);
}
