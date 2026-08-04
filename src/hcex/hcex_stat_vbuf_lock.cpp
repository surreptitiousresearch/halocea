/* hcex_stat_vbuf_lock @0x823EFB38 — lock a static defrag-managed vertex buffer chunk for writing and hand
 * back a CPU pointer at the requested byte offset. Returns the number of bytes already written into the
 * chunk (ptr - pBuf->ptr). The `size` argument is accepted for symmetry with the dynamic path but is unused
 * here — the whole chunk is already reserved. apDEFRAG_CHUNK is a ws-engine boundary type. */

#include "../headers/apDEFRAG_CHUNK.h"

extern "C" int hcex_stat_vbuf_lock(apDEFRAG_CHUNK *static_vbuf, int ofs, int size, void **buf_ptr)
{
    (void)size;
    char *base = (char *)static_vbuf->__vftable->LockWrite(static_vbuf); /* vtbl slot 2 */
    *buf_ptr = base + ofs;
    return (int)(static_vbuf->ptr - static_vbuf->pBuf->ptr);
}
