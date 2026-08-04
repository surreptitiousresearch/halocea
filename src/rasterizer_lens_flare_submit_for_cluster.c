/* rasterizer_lens_flare_submit_for_cluster @0x83792628 — submit every lens-flare marker belonging to one BSP
 * cluster to the frame's lens-flare renderer. Gated off entirely unless the draw_lens_flares debug option is set,
 * and skipped while a multi-tile screenshot is being assembled. For each of the cluster's markers it reads the
 * marker's world position and its signed-byte compressed direction (each axis / 127), builds an up vector via
 * perpendicular3d, normalizes both (leaving near-zero vectors alone), compresses them back to packed ints, and
 * fills a submit_parameters record with the marker's lens_flare tag definition (resolved through TAG_INSTANCE
 * from the referenced lens_flares element) before handing it to rasterizer_lens_flare_submit.
 *
 * DEVIATION: Hex-Rays punned the three direction bytes through an __int128 (v9) with DWORD/LODWORD splits;
 * disasm (0x83792710-0x83792778) settles the mapping to signed bytes marker[0xC/0xD/0xE] -> n[0/1/2] scaled by
 * 0x3C010204 (== 1/127). The marker/lens_flare tag_block fields live inside structure_bsp's former gap (now
 * broken out at 0x11C/0x128). The `*(_DWORD *)&light_index = marker_index` the decompiler emitted is a big-endian
 * 32-bit store spanning light_index@0x1E and lens_flare_index@0x20; reproduced as the two field writes per
 * disasm (srawi for the high half -> light_index, low half -> lens_flare_index @0x83792824/0x8379286C). */

#include <stdint.h>
#include "headers/structure_bsp.h"
#include "headers/structure_cluster.h"
#include "headers/structure_lens_flare_marker.h"
#include "headers/structure_lens_flare.h"
#include "headers/rasterizer_lens_flare_submit_parameters.h"
#include "headers/lens_flare_definition.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/render_globals.h"
#include "headers/global_tag_instances.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"
#include "headers/datum_index.h"
extern float __fsqrts(float);


extern real_vector3d *perpendicular3d(const real_vector3d *a, real_vector3d *result);
extern unsigned int compress_real_vector3d_to_int32_clamp(const real_vector3d *v);
extern void rasterizer_lens_flare_submit(const rasterizer_lens_flare_submit_parameters *parameters);

void rasterizer_lens_flare_submit_for_cluster(int16_t cluster_index)
{
    if ( !rasterizer_debug_options.draw_lens_flares )
        return;
    /* skip while a multi-tile screenshot is being assembled */
    if ( global_screenshot_count > 1 || (global_screenshot_count == 1 && global_screenshot_size > 1) )
        return;

    const structure_bsp *bsp = global_structure_bsp;
    const structure_cluster *cluster = (const structure_cluster *)bsp->clusters.address + cluster_index;

    unsigned __int16 marker_count = cluster->lens_flare_marker_count;
    unsigned __int16 first_marker = cluster->first_lens_flare_marker_index;

    for ( int i = 0; i < marker_count; ++i )
    {
        int marker_index = first_marker + i;
        const structure_lens_flare_marker *marker = (const structure_lens_flare_marker *)bsp->lens_flare_markers.address + marker_index;
        const structure_lens_flare *lens_flare = (const structure_lens_flare *)bsp->lens_flares.address + marker->lens_flare_index;

        real_vector3d direction;
        direction.n[0] = (float)(signed char)marker->i_direction * 0.0078740157f;
        direction.n[1] = (float)(signed char)marker->j_direction * 0.0078740157f;
        direction.n[2] = (float)(signed char)marker->k_direction * 0.0078740157f;

        real_vector3d up[2];
        perpendicular3d(&direction, up);

        float dir_mag = __fsqrts(direction.n[0] * direction.n[0]
                + direction.n[1] * direction.n[1] + direction.n[2] * direction.n[2]);
        if ( __fabs(dir_mag) >= 0.000099999997f )
        {
            float inverse = 1.0f / dir_mag;
            direction.n[0] *= inverse;
            direction.n[1] *= inverse;
            direction.n[2] *= inverse;
        }

        float up_mag = __fsqrts(up[0].n[0] * up[0].n[0]
                + up[0].n[1] * up[0].n[1] + up[0].n[2] * up[0].n[2]);
        if ( __fabs(up_mag) >= 0.000099999997f )
        {
            float inverse = 1.0f / up_mag;
            up[0].n[0] *= inverse;
            up[0].n[1] *= inverse;
            up[0].n[2] *= inverse;
        }

        rasterizer_lens_flare_submit_parameters params;
        params.compressed_direction = compress_real_vector3d_to_int32_clamp(&direction);
        params.compressed_up = compress_real_vector3d_to_int32_clamp(up);
        params.position.n[0] = marker->position.n[0];
        params.position.n[1] = marker->position.n[1];
        params.position.n[2] = marker->position.n[2];
        params.compressed_light_color = -1;
        params.compressed_light_scale = 0;
        params.light_identifier = -1;
        params.definition = TAG_GET(lens_flare_definition, lens_flare->lens_flare.index);
        params.light_index = (__int16)DATUM_INDEX_TO_IDENTIFIER(marker_index);
        params.lens_flare_index = (__int16)marker_index;
        params.compressed_window_index = (unsigned char)render.window_index;
        rasterizer_lens_flare_submit(&params);
    }
}
