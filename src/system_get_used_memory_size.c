/* system_get_used_memory_size @0x83762EC0 — tail-call forwarder (`b LocalSize`; confirmed via disasm,
 * not a bare blr). Win32 boundary — declared extern per the project's Win32 boundary convention, not
 * reversed; called through with the same argument. */

extern unsigned int LocalSize(void *hMem);

unsigned int system_get_used_memory_size(void *pointer)
{
    return LocalSize(pointer);
}
