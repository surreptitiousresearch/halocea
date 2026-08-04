/* pool_free @0x83730668 — tail-call forwarder (`b dlFree`; confirmed via disasm, not a bare blr).
 * Blam's generic pool allocator is backed directly by dlmalloc's free() in this build. dlFree is a
 * CRT/runtime boundary — declared extern inline per the project's CRT boundary convention, not
 * reversed. */

extern void dlFree(void *ptr);

void pool_free(void *pointer)
{
    dlFree(pointer);
}
