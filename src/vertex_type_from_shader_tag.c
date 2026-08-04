/* vertex_type_from_shader_tag @0x837B4928 — pick the rasterizer vertex-buffer type (and its lightmap
 * companion) for an environment shader. This build's mapping ignores group_tag entirely (confirmed via
 * disasm: r3 is never read) and keys only on whether the geometry uses compressed vertices. Enum values
 * DB-verified against the compiled rasterizer_vertex_type enum ($CF57790460AB4D75BF9117BCF4018910). */

#include <stdint.h>
#include "headers/rasterizer_vertex_type.h"

void vertex_type_from_shader_tag(unsigned int group_tag, int16_t *vertex_type, int16_t *lightmap_vertex_type, uint8_t compressed)
{
    if ( compressed )
    {
        *vertex_type = _rasterizer_vertex_type_environment_compressed;
        *lightmap_vertex_type = _rasterizer_vertex_type_environment_lightmap_compressed;
    }
    else
    {
        *vertex_type = _rasterizer_vertex_type_environment_uncompressed;
        *lightmap_vertex_type = _rasterizer_vertex_type_environment_lightmap_uncompressed;
    }
}
