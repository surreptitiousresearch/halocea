/* system_calloc @0x83762E30 — tail-call forwarder (`mullw r4, r3, r4; li r3, 0x40; b GlobalAlloc`; confirmed
 * via disasm, not a bare blr). GMEM_ZEROINIT (0x40) matches calloc's zero-fill contract. */

extern void *GlobalAlloc(unsigned int uFlags, unsigned int dwBytes);

void *system_calloc(unsigned int num, unsigned int size)
{
    return GlobalAlloc(0x40, num * size);
}
