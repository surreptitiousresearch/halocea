/* system_free @0x83762E50 — tail-call forwarder (`b LocalFree`; confirmed via disasm, not a bare blr).
 * Blam's generic system-heap free is backed directly by the Win32 LocalFree API in this build. LocalFree
 * is a Win32 boundary — declared extern per the project's Win32 boundary convention, not reversed;
 * its non-void return (the freed handle on failure) is discarded, matching this function's own void
 * signature. */

extern void *LocalFree(void *hMem);

void system_free(void *pointer)
{
    LocalFree(pointer);
}
