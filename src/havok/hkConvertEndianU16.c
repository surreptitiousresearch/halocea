/* hkConvertEndianU16 @0x838BDBA8 */
/* 16-bit byte swap. The decompiler renders this as a byte-by-byte shuffle
   through a stack slot; it is a plain endian swap of a 16-bit value. */
int hkConvertEndianU16(unsigned short n)
{
    return (unsigned short)((n >> 8) | (n << 8));
}
