/* hkConvertEndianU32 @0x838BDB78 */
/* 32-bit byte swap. */
unsigned int hkConvertEndianU32(unsigned int n)
{
    return (n >> 24) | ((n >> 8) & 0xFF00) | ((n << 8) & 0xFF0000) | (n << 24);
}
