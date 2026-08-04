/* hcex_stat_vbuf_free @0x823EFBF8 — release an HCEX static vertex/index buffer back to the ws-engine defrag
 * manager. apDEFRAG_MNG is a ws-engine boundary type. */
typedef struct apDEFRAG_CHUNK apDEFRAG_CHUNK;
typedef struct apDEFRAG_MNG apDEFRAG_MNG;
extern apDEFRAG_MNG *apDefragMng;
extern void apDEFRAG_MNG_Free(apDEFRAG_MNG *self, apDEFRAG_CHUNK *chunk);
extern "C" void hcex_stat_vbuf_free(apDEFRAG_CHUNK *vbuf)
{
    apDEFRAG_MNG_Free(apDefragMng, vbuf);
}
