/* rasterizer_geometry_compress_vertices @0x837E8418 — pack an array of `count` full-precision vertices into
 * their compressed GPU vertex format, dispatched by vertex `type`. Positions/normals/tangents that are stored
 * as real_vector3d are packed to a single int32 via compress_real_vector3d_to_int32_clamp; texture coordinates
 * and other [-1,1] scalars are packed to signed int16 via floor(clamp(v,-1,1) * 32767.5). Three formats are
 * handled: type 2 (a compact 8-byte-stride vertex: one packed vector + two packed int16 coords), type 4 (a
 * 32-byte-stride skinned/rigid vertex: three raw dwords + three packed vectors + several packed coords + two
 * bone weights scaled by 3), and the default type 0 (a 32-byte-stride vertex: three raw dwords + three packed
 * vectors + two raw trailing dwords). The compressed/uncompressed sizes are unused (the strides are fixed per
 * type). Sibling of rasterizer_geometry_byte_swap_vertices.
 *
 * DEVIATION: register-pun long doubles for the floor() soft-float pairs are single-precision float math;
 * reproduced with float. Pointer strides transcribed from disasm (uncompressed float* advances of 5/17/14
 * elements = 20/68/56 bytes; compressed advances of 8/32/32 bytes). */

#include <stdint.h>
#include "headers/real_vector3d.h"
#include "headers/rasterizer_vertex_type.h"
#include <math.h>

extern unsigned int compress_real_vector3d_to_int32_clamp(const real_vector3d *v);
/* floor provided by CRT via <math.h>; local extern removed (C28251: the local redeclaration drops the header's annotations) */

static int16_t pack_unit_int16(float value)
{
    float clamped;
    if ( value >= -1.0f )
        clamped = value > 1.0f ? 1.0f : value;
    else
        clamped = -1.0f;
    return (int16_t)(int)(float)floor((clamped * 32767.5f));
}

void rasterizer_geometry_compress_vertices(int16_t type, int count, char *compressed, int compressed_size, char *uncompressed, int uncompressed_size)
{
    if ( type == _rasterizer_vertex_type_environment_lightmap_uncompressed )
    {
        if ( count <= 0 )
            return;
        char *dst = compressed - 2;
        float *src = (float *)(uncompressed + 16); /* RAW (irreducible): raw uncompressed vertex buffer, no tag struct — packed GPU vertex stream */
        for ( int i = count; i; --i )
        {
            /* dst/src are raw packed GPU vertex buffers (no struct) — faithful de-flagged writes */
            ((int *)(dst + 2))[0] = compress_real_vector3d_to_int32_clamp((const real_vector3d *)(src - 4)); /* RAW (irreducible): raw packed GPU vertex, no tag struct */
            ((int16_t *)dst)[3] = pack_unit_int16(src[-1]);
            int packed = (int)(float)floor(((src[0] >= -1.0f ? (src[0] > 1.0f ? 1.0f : src[0]) : -1.0f)
                    * 32767.5f));
            src += 5;
            dst += 8;
            *(int16_t *)dst = (unsigned int)packed >> 16;
        }
    }
    else if ( type == _rasterizer_vertex_type_model_uncompressed )
    {
        if ( count <= 0 )
            return;
        char *dst = compressed + 16;
        char *src = uncompressed + 24;
        for ( int i = count; i; --i )
        {
            *((int *)dst - 4) = *((int *)src - 6);
            *((int *)dst - 3) = *((int *)src - 5);
            *((int *)dst - 2) = *((int *)src - 4);
            *((int *)dst - 1) = compress_real_vector3d_to_int32_clamp((const real_vector3d *)src - 1);
            *(int *)dst = compress_real_vector3d_to_int32_clamp((const real_vector3d *)src);
            ((int *)dst)[1] = compress_real_vector3d_to_int32_clamp((const real_vector3d *)src + 1);
            ((int16_t *)dst)[4] = pack_unit_int16(((float *)src)[6]);
            ((int16_t *)dst)[5] = pack_unit_int16(((float *)src)[7]);
            dst[12] = 3 * ((int16_t *)src)[16];
            dst[13] = 3 * ((int16_t *)src)[17];
            int packed = (int)(float)floor(((((float *)src)[9] >= -1.0f
                    ? (((float *)src)[9] > 1.0f ? 1.0f : ((float *)src)[9]) : -1.0f) * 32767.5f));
            src += 68;
            ((int16_t *)dst)[7] = (unsigned int)packed >> 16;
            dst += 32;
        }
    }
    else
    {
        if ( count <= 0 )
            return;
        int *dst = (int *)(compressed + 16); /* RAW (irreducible): raw packed GPU vertex buffer, no tag struct */
        char *src = uncompressed + 24;
        for ( int i = count; i; --i )
        {
            *(dst - 4) = *((int *)src - 6);
            *(dst - 3) = *((int *)src - 5);
            *(dst - 2) = *((int *)src - 4);
            *(dst - 1) = compress_real_vector3d_to_int32_clamp((const real_vector3d *)src - 1);
            *dst = compress_real_vector3d_to_int32_clamp((const real_vector3d *)src);
            dst[1] = compress_real_vector3d_to_int32_clamp((const real_vector3d *)src + 1);
            dst[2] = ((int *)src)[6];
            dst[3] = ((int *)src)[7];
            src += 56;
            dst += 8;
        }
    }
}
