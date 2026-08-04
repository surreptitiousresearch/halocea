/* DEVIATION: disasm: fctidz + lbz at back_chain+7 (LSByte of 64-bit result) + rotrwi 8 = value << 24 */
unsigned int real_alpha_to_pixel32(float alpha)
{
    return (unsigned int)((unsigned char)(int)(alpha * 255.0f)) << 24;
}
