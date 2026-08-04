/* hcexRenderInit @0x823F2400 — wires Blam's rasterizer output into the ws-engine render job
 * pipeline: sets the fixed HCEX render-target resolution, registers the HCEX defrag pool/chunk
 * kind with the shared defrag manager, then reserves its backing buffer. Called once from
 * haloInit when running in Halo mode. */

#include "../headers/ws/ap/apDEFRAG_MNG.h"
#include "../headers/ws/vid/vidDEFRAG_POOL_HCEX_X360.h"
#include "../headers/hcex/vidDEFRAG_CHUNK_OBJ_HCEX_X360.h"

extern int            hcex_screen_width;
extern int            hcex_screen_height;
extern apDEFRAG_MNG  *apDefragMng;

void hcexRenderInit()
{
    hcex_screen_width = 1280;
    hcex_screen_height = 720;

    apDefragMng->RegisterPool<vidDEFRAG_POOL_HCEX_X360>();
    apDefragMng->RegisterChunk<vidDEFRAG_CHUNK_OBJ_HCEX_X360>("hcex_x360", nullptr);
    apDefragMng->AllocBuf("hcex_x360", 0);
}
