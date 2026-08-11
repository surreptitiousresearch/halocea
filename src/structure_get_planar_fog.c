/* structure_get_planar_fog @0x83744B38 — gather the planar/screen fog affecting a given structure cluster
 * into the render fog block. The cluster's fog definition is looked up (preferring the primary fog, then the
 * "screen" fog); a screen-fog definition just latches its flags and screen parameters, while a planar-fog
 * definition fills in the fog plane (from the cluster's referenced fog plane, or planar_mode 2 when the cluster
 * has none), the planar color and the density/depth/distance limits. When a fog plane is present a (currently
 * zero) world-space offset is also pushed to the renderer.
 *
 * DEVIATION: the original reads the definition's leading dword as a float then takes its low word for
 * fog_definition_flags; reproduced as (unsigned __int16) of the leading int. The fog-offset scale multiplies by
 * a literal 0.0 in the binary (a disabled feature) — preserved verbatim. */

#include <stdint.h>
#include "headers/render_fog.h"
#include "headers/render_fog_runtime_flags.h"
#include "headers/fog_definition.h"
#include "headers/structure_bsp.h"
#include "headers/structure_cluster.h"
#include "headers/structure_fog_plane.h"
#include "headers/global_tag_instances.h"
#include "headers/render_planar_fog_mode.h"
#include "headers/real_vector3d.h"

extern int structure_get_planar_fog_definition_index(const structure_bsp *structure, int16_t cluster_index, uint8_t get_fog_from_sky);
extern void structure_render_set_fog_offset(real_vector3d *vector_offset);

void structure_get_planar_fog(int16_t cluster_index, render_fog *fog)
{
    const structure_bsp *structure = global_structure_bsp;
    uint8_t is_screen_fog = 0;

    fog->planar_mode = 0;
    fog->fog_definition_flags = 0;
    fog->screen = nullptr;

    int fog_definition_index = structure_get_planar_fog_definition_index(structure, cluster_index, 0);
    if (fog_definition_index == -1)
    {
        fog_definition_index = structure_get_planar_fog_definition_index(structure, cluster_index, 1);
        is_screen_fog = 1;
    }
    if (fog_definition_index == -1)
        return;

    const structure_cluster *cluster =
        &((const structure_cluster *)global_structure_bsp->clusters.address)[cluster_index];
    fog_definition *definition = TAG_GET(fog_definition, fog_definition_index);

    if (is_screen_fog)
    {
        fog->runtime_flags |= (1u << _render_fog_runtime_screen_use_sky_interpolator_bit);
        fog->fog_definition_flags = definition->flags;
        fog->screen = &definition->screen; /* fog tag +0x84 */
        return;
    }

    uint16_t cluster_fog_plane = (uint16_t)cluster->fog_designator;
    if ((cluster_fog_plane & 0x8000) != 0)
    {
        fog->planar_mode = _render_planar_fog_mode_normal;
        /* (32*d) & 0xFFFE0 == (d & 0x7FFF)*32 — strip the 0x8000 designator flag, index the 32-byte fog planes */
        const structure_fog_plane *fog_plane =
            &((const structure_fog_plane *)global_structure_bsp->fog_planes.address)[cluster_fog_plane & 0x7FFF];
        fog->plane.n.n[0] = fog_plane->plane.n.n[0];
        fog->plane.n.n[1] = fog_plane->plane.n.n[1];
        fog->plane.n.n[2] = fog_plane->plane.n.n[2];
        fog->plane.d = fog_plane->plane.d;
    }
    else
    {
        fog->planar_mode = _render_planar_fog_mode_fully_fogged;
    }

    fog->planar_color.red = definition->color.red;
    fog->planar_color.green = definition->color.green;
    fog->planar_color.blue = definition->color.blue;
    fog->planar_maximum_density = definition->maximum_density;
    fog->planar_maximum_depth = definition->maximum_depth;
    fog->planar_maximum_distance = definition->maximum_distance;

    if ((cluster_fog_plane & 0x8000) != 0)
    {
        float offset_scale = definition->animation_distance * 0.0f;
        fog->plane.d = fog->plane.d + offset_scale;
        real_vector3d offset;
        offset.n[0] = fog->plane.n.n[0] * offset_scale;
        offset.n[1] = fog->plane.n.n[1] * offset_scale;
        offset.n[2] = fog->plane.n.n[2] * offset_scale;
        structure_render_set_fog_offset(&offset);
    }

    fog->fog_definition_flags = (uint16_t)definition->flags;
    fog->screen = &definition->screen; /* fog tag +0x84 */
}
