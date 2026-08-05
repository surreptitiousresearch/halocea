/* hcex_get_dyn_chunk @0x823EFC38 — lazily grab (and cache) a chunk from the vid driver's dynamic-geometry
 * hardware buffer for this frame's dynamic-geometry writes. Returns the cached chunk on subsequent calls
 * within the same frame; the magic tag 0x4E495645 (fourcc 'NIVE' / build-specific chunk id) matches
 * the value the driver's Begin() expects. vidDriver / dynGeomBuffer are ws-engine boundaries. */

#include "../headers/vidDRIVER_DYNGEOM_BUFFER.h"

extern vidDRIVER *vidDriver;
extern vidDRIVER_DYNGEOM_CHUNK *hcex_dyn_chunk;

extern int IGNORE_STRONG_ASSERT; /* .data @0x841DB148 - ?IGNORE_STRONG_ASSERT@@3HA (def: src/data/IGNORE_STRONG_ASSERT.cpp) */
extern "C" const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */
extern void STRONG_ASSERT_DUMMY_Crash(void *ctx, const char *expr, const char *file, int line, const char *info);

vidDRIVER_DYNGEOM_CHUNK *hcex_get_dyn_chunk(void)
{
    vidDRIVER_DYNGEOM_CHUNK *chunk = hcex_dyn_chunk;

    if ( !chunk )
    {
        vidDRIVER_DYNGEOM_BUFFER *dynGeomBuffer = vidDriver->drvInterface->dynGeomBuffer;
        chunk = dynGeomBuffer->__vftable->Begin(dynGeomBuffer, 0x4E495645u /* 'NIVE' chunk id */);
        hcex_dyn_chunk = chunk;

        if ( !IGNORE_STRONG_ASSERT && !chunk )
        {
            STRONG_ASSERT_DUMMY_Crash(0, "hcex_dyn_chunk", "D:\\Projects\\code\\HCEX\\sources\\halo_render.cpp",
                                      404, empty_string);
            return hcex_dyn_chunk;
        }
    }

    return chunk;
}
