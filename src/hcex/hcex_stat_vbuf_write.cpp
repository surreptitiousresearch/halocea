/* hcex_stat_vbuf_write @0x823EFB90 — write vertex data into a static defrag-managed buffer chunk (at chunk
 * offset 0) and return the resulting byte offset within the owning buffer (ptr - pBuf->ptr). Mirrors
 * hcex_stat_ibuf_write but returns the write cursor. vidVBUF_MNG is a ws-engine boundary; vidVBMng is its
 * global instance. apDEFRAG_CHUNK is the ws-engine defrag chunk (boundary). */

#include "../headers/apDEFRAG_CHUNK.h"

typedef struct vidVBUF_MNG vidVBUF_MNG;
typedef struct vidDEFRAG_CHUNK vidDEFRAG_CHUNK;

extern vidVBUF_MNG *vidVBMng;
extern void vidVBUF_MNG_WriteStatData(vidVBUF_MNG *self, vidDEFRAG_CHUNK *chunk,
                                       int offset, const void *data, int size); /* ?WriteStatData@vidVBUF_MNG@@QAAXPAUvidDEFRAG_CHUNK@@HPBXH@Z @0x827B8BC8 */

extern "C" int hcex_stat_vbuf_write(apDEFRAG_CHUNK *static_vbuf, const void *src, int size)
{
    /* apDEFRAG_CHUNK is the layout view this TU derefs (ptr/pBuf); the manager takes it as vidDEFRAG_CHUNK */
    vidVBUF_MNG_WriteStatData(vidVBMng, (vidDEFRAG_CHUNK *)static_vbuf, 0, src, size);
    return (int)(static_vbuf->ptr - static_vbuf->pBuf->ptr);
}
