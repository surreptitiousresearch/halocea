/* system_malloc @0x83762E40 — tail-call forwarder (`mr r4, r3; li r3, 0; b GlobalAlloc`; confirmed via
 * disasm, not a bare blr). */

extern void *GlobalAlloc(unsigned int uFlags, unsigned int dwBytes);

void *system_malloc(unsigned int size)
{
    return GlobalAlloc(0, size);
}
