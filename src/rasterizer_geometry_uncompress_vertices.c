/* rasterizer_geometry_uncompress_vertices 0x837E8718 — expand a packed geometry vertex buffer into
 * its full float representation. Dispatched on the vertex-declaration `type`: each type selects a
 * compressed source layout and its matching uncompressed destination layout, then decodes every
 * vertex. Normal/binormal/tangent are 11-11-10 packed dwords decoded by
 * uncompress_int32_to_real_vector3d; texcoords are signed-16 quantised; positions are stored
 * uncompressed and copied verbatim.
 *
 * CAVEAT — reconstruction notes:
 *  - SIGNATURE: the decompiler failed local allocation and fused each (pointer, size) argument pair
 *    into one __int64 (reading the pointer out of the HIDWORD). The real 6-parameter prototype is
 *    taken from the database:
 *      (__int16 type, int count, void *uncompressed, int uncompressed_size,
 *       const void *compressed, int compressed_size).
 *    The two *_size arguments are unreferenced in the release body (bounds/asserts stripped).
 *  - LOOP SHAPE: the compiler unrolled the type-1 and type-3 loops by four (with a scalar remainder
 *    tail); type-5 was a simple loop. All three are reconstructed here in their natural per-vertex
 *    loop form — semantically identical, and matching the obvious original source.
 *  - TYPE -> FORMAT mapping (source/dest struct sizes DB-verified):
 *      type 1: environment_vertex_compressed(32)          -> environment_vertex_uncompressed(56)
 *      type 3: environment_lightmap_vertex_compressed(8)  -> environment_lightmap_vertex_uncompressed(20)
 *      type 5: model_vertex_compressed(32)                -> model_vertex_uncompressed(68)
 *  - The 11-11-10 field decode is not inlined here; it is delegated to
 *    uncompress_int32_to_real_vector3d (which the type-1/type-3 branches inlined and type-5 called),
 *    keeping the three branches faithful to that shared helper. */

#include <stdint.h>
#include "headers/real_vector3d.h"
#include "headers/rasterizer_vertex_type.h"
#include "headers/environment_vertex_compressed.h"
#include "headers/environment_vertex_uncompressed.h"
#include "headers/environment_lightmap_vertex_compressed.h"
#include "headers/environment_lightmap_vertex_uncompressed.h"
#include "headers/model_vertex_compressed.h"
#include "headers/model_vertex_uncompressed.h"

/* boundary — reversed sibling: decode a packed 11-11-10 signed vector into three floats. */
#include "headers/real_vector3d.h"
extern real_vector3d * uncompress_int32_to_real_vector3d(real_vector3d *result, unsigned int i);

/* signed-16 texcoord quantum: (2 * raw + 1) / 65535 maps [-32768, 32767] -> [-1, 1] */
#define UNCOMPRESS_TEXCOORD_16(raw) \
    ((float)((float)((float)(raw) * 2.0f) + 1.0f) * 0.000015259022f)

void rasterizer_geometry_uncompress_vertices(int16_t type, int count, void *uncompressed, int uncompressed_size, const void *compressed, int compressed_size)
{
    (void)uncompressed_size;
    (void)compressed_size;

    if ( type == _rasterizer_vertex_type_environment_compressed )
    {
        /* environment structure vertices */
        const environment_vertex_compressed *source = compressed;
        environment_vertex_uncompressed *destination = uncompressed;
        int vertex_index;

        for ( vertex_index = 0; vertex_index < count; ++vertex_index )
        {
            destination[vertex_index].position = source[vertex_index].position;
            uncompress_int32_to_real_vector3d(&destination[vertex_index].normal,   source[vertex_index].normal);
            uncompress_int32_to_real_vector3d(&destination[vertex_index].binormal, source[vertex_index].binormal);
            uncompress_int32_to_real_vector3d(&destination[vertex_index].tangent,  source[vertex_index].tangent);
            destination[vertex_index].texcoord = source[vertex_index].texcoord;
        }
    }
    else if ( type == _rasterizer_vertex_type_environment_lightmap_compressed )
    {
        /* environment lightmap vertices */
        const environment_lightmap_vertex_compressed *source = compressed;
        environment_lightmap_vertex_uncompressed *destination = uncompressed;
        int vertex_index;

        for ( vertex_index = 0; vertex_index < count; ++vertex_index )
        {
            uncompress_int32_to_real_vector3d(
                &destination[vertex_index].incident_radiosity,
                source[vertex_index].incident_radiosity);
            destination[vertex_index].texcoord.n[0] = UNCOMPRESS_TEXCOORD_16(source[vertex_index].lightmap_u);
            destination[vertex_index].texcoord.n[1] = UNCOMPRESS_TEXCOORD_16(source[vertex_index].lightmap_v);
        }
    }
    else if ( type == _rasterizer_vertex_type_model_compressed && count > 0 )
    {
        /* skinned model vertices */
        const model_vertex_compressed *source = compressed;
        model_vertex_uncompressed *destination = uncompressed;
        int vertex_index;

        for ( vertex_index = 0; vertex_index < count; ++vertex_index )
        {
            unsigned int quantised_weight;

            destination[vertex_index].position = source[vertex_index].position;
            uncompress_int32_to_real_vector3d(&destination[vertex_index].normal,   source[vertex_index].normal);
            uncompress_int32_to_real_vector3d(&destination[vertex_index].binormal, source[vertex_index].binormal);
            uncompress_int32_to_real_vector3d(&destination[vertex_index].tangent,  source[vertex_index].tangent);

            destination[vertex_index].texcoord.n[0] = UNCOMPRESS_TEXCOORD_16(source[vertex_index].texcoord_u);
            destination[vertex_index].texcoord.n[1] = UNCOMPRESS_TEXCOORD_16(source[vertex_index].texcoord_v);

            /* palette indices are stored as index*3 (per-bone matrix stride); recover the index */
            destination[vertex_index].nodes[0] = source[vertex_index].nodes[0] / 3;
            destination[vertex_index].nodes[1] = source[vertex_index].nodes[1] / 3;

            /* weight is a single quantised byte in the low half of weights[0]; the pair sums to 1 */
            quantised_weight = (uint8_t)source[vertex_index].weights[0];
            destination[vertex_index].weights[0] = (float)quantised_weight * 0.0039215689f;   /* / 255 */
            destination[vertex_index].weights[1] = 1.0f - (float)quantised_weight * 0.0039215689f;
        }
    }
}
