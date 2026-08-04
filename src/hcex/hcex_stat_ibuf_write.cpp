/* hcex_stat_ibuf_write @ 0x823EFC08
   Writes index data into a static defrag-managed buffer chunk at offset 0.
   vidVBUF_MNG is the ws-engine video vertex/index buffer manager (boundary);
   vidVBMng is its global instance. */

typedef struct vidVBUF_MNG vidVBUF_MNG;
typedef struct vidDEFRAG_CHUNK vidDEFRAG_CHUNK;

extern vidVBUF_MNG *vidVBMng;
extern void vidVBUF_MNG_WriteStatData(vidVBUF_MNG *self, vidDEFRAG_CHUNK *chunk,
                                       int offset, const void *data, int size);

extern "C" void hcex_stat_ibuf_write(vidDEFRAG_CHUNK *static_vbuf, const void *src, int size)
{
    vidVBUF_MNG_WriteStatData(vidVBMng, static_vbuf, 0, src, size);
}
