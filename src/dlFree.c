void apDLALLOC_IFACE_Free(void *ptr);
int  __apMemoryDebugRemove(void *ptr, int is_allocator); /* mangled _N: bool, passed as 1 in r4 */

void dlFree(void *ptr)
{
    if (ptr)
    {
        __apMemoryDebugRemove(ptr, 1);
        apDLALLOC_IFACE_Free(ptr);
    }
}
