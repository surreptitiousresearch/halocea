/* hcex_init_effect @0x836823B0 — bridge a Blam effect spawn into the HCEX (ws-engine) effect system.
 * Copies the per-marker positions and forward vectors into plain hcex_float3 arrays and forwards them,
 * with the effect tag name, to hcex_create_effect. Up to 32 markers are supported.
 *
 * NOTE: the original compiles the array copies as a 4-wide SIMD-unrolled loop with heavily obfuscated
 * pointer arithmetic; reconstructed here as the equivalent straight per-element copy. */

#include <stdint.h>
#include "../headers/hcex/hcex_float3.h"
#include "../headers/real_point3d.h"
#include "../headers/real_vector3d.h"

extern "C" char *tag_get_name(int tag_index);
extern "C" void hcex_output_dbg(const char *text, ...);
extern "C" void hcex_pix_begin_event(const char *text, ...);
extern "C" void hcex_pix_end_event(void);
extern "C" void hcex_create_effect(const char *name, int obj_follow, int plr_idx, hcex_float3 *positions,
        hcex_float3 *vectors, const char **names, int npoints);

extern "C" void hcex_init_effect(int definition_index, int obj_follow, int plr_idx, real_point3d *points,
        real_vector3d *vectors, const char **names, int npoints, float scale)
{
    const char *name;
    hcex_float3 positions[32];
    hcex_float3 forward_vectors[32];
    int i;

    if ( definition_index == -1 )
        return;

    name = tag_get_name(definition_index);
    if ( !name )
        return;

    if ( npoints > 32 )
    {
        hcex_output_dbg("Too much points (%d) in hcex_init_effect for '%s'\n", npoints, name);
        return;
    }
    if ( npoints < 1 )
    {
        hcex_output_dbg("Not enough points in hcex_init_effect for '%s'\n", name);
        return;
    }

    for ( i = 0; i < npoints; ++i )
    {
        positions[i].x = points[i].n[0];
        positions[i].y = points[i].n[1];
        positions[i].z = points[i].n[2];
        forward_vectors[i].x = vectors[i].n[0];
        forward_vectors[i].y = vectors[i].n[1];
        forward_vectors[i].z = vectors[i].n[2];
    }

    hcex_pix_begin_event("hcex_create_effect");
    hcex_create_effect(name, obj_follow, plr_idx, positions, forward_vectors, names, npoints);
    hcex_pix_end_event();
}
