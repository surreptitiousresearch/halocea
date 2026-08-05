/* breakable_surface_effect @0x83743190 — spawn the visual/audio fracture effect for a breakable surface
 * that has just been broken by a damage event. Starting from the seeded collision-BSP surface, it flood-
 * fills the connected group of surfaces that share the same breakable-surface index and material (walking
 * the per-surface edge loops), projecting each surface onto its dominant coordinate plane to build a 2D
 * convex outline and 3D/2D bounding boxes. For each particle emitter in the material's breakable-surface
 * effect block it tiles a jittered grid of spawn points across the 2D outline, and for every point inside
 * the outline spawns a debris particle whose direction/velocity/rotation/radius/color are derived from the
 * emitter definition and the damage falloff. Finally it plays the material's impulse break sound at the
 * center of the surface group's 3D bounds.
 *
 * Geometry: per surface the first edge establishes a 2D frame — u_axis = normalized first-edge direction,
 * v_axis = u_axis x plane_normal, origin = first vertex (or, for the seed surface, the damage epicenter
 * projected onto the plane). A vertex's 2D coords are ((vertex-origin)·u_axis, (vertex-origin)·v_axis).
 *
 * collision surface (12 bytes): plane_designator@0 (sign bit selects plane orientation), flags@8,
 *   breakable_surface_index@9, material_index@10, first_edge@4.
 * collision edge (24 bytes): vertex_a@0, vertex_b@4, surface_a_next_edge@8, surface_b_next_edge@12,
 *   surface_a@16, surface_b@20.
 * collision vertex (16 bytes): position@0/4/8.
 * material breakable-surface effect block: sound def@+44, emitter count@+72, emitters ptr@+76.
 * emitter (128 bytes): particle def@12, color flags@16, grid spacing@20, velocity range@24/28,
 *   angular velocity range@36/40, radius range@52/56, intensity range@68/84, rgb low@72, rgb high@88. */

#include <stdint.h>
#include "headers/collision_bsp.h"
#include "headers/collision_edge.h"
#include "headers/structure_bsp.h"
#include "headers/game_globals_tag.h"
#include "headers/material_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/collision_surface.h"
#include "headers/breakable_surface.h"
#include "headers/breakable_surface_particle_effect.h"
#include "headers/new_particle_data.h"
#include "headers/damage_data.h"
#include "headers/damage_effect_definition.h"
#include "headers/sound_source.h"
#include "headers/real_point2d.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_rgb_color.h"
#include "headers/breakable_surface_particle_effect_color_interpolate_flags.h"
#include "headers/math_constants.h"
#include <math.h>
#include "headers/blam_data_globals.h"
extern float __fsqrts(float);

extern const int16_t global_projection3d_mappings[1][6][2];

extern void hcex_kill_breakable_surface(int bsp, int idx);
extern uint32_t *get_global_local_random_seed_address(void);
extern float real_seed_random(uint32_t *seed);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);
extern uint8_t convex_hull2d_test_point(int16_t count, const real_point2d *points, const real_point2d *point, float epsilon);
extern real_rgb_color *rgb_colors_interpolate(real_rgb_color *rgb_result, unsigned int flags, const real_rgb_color *rgb_lower_bound, const real_rgb_color *rgb_upper_bound, float u);
extern real_vector3d *seed_random_direction3d(uint32_t *seed, real_vector3d *direction);
extern void particle_new(const new_particle_data *data);
extern int unattached_impulse_sound_new(int definition_index, const sound_location *location, float scale, uint8_t is_player);

void breakable_surface_effect(int16_t breakable_surface_index, const damage_data *damage_data,
        int seed_surface_index)
{
    collision_bsp *collision = global_collision_bsp;
    structure_bsp *structure = global_structure_bsp;
    extern int16_t global_structure_bsp_index;

    hcex_kill_breakable_surface(global_structure_bsp_index, breakable_surface_index);
    if ( !breakable_surface_effect_enabled )
        return;

    {
    const int16_t *collision_materials = (const int16_t *)structure->collision_materials.address;
    const collision_surface *seed_surface =
        &((const collision_surface *)collision->surfaces.address)[seed_surface_index];
    int material_global_index = collision_materials[10 * seed_surface->material_index + 9];
    breakable_surface *effect_block =
        &((material_definition *)global_game_globals->materials.address)[material_global_index].breakable_surface;

    int surface_worklist[76];
    int16_t worklist_count = 1;
    int16_t worklist_index = 0;
    int seeded_index = seed_surface_index;

    /* 2D outline / bounds accumulators across the whole surface group */
    real_point2d hull_points[8];
    float min_u = 0.0f, max_u = 0.0f, min_v = 0.0f, max_v = 0.0f;       /* per-surface 2D bbox */
    float min_x = 0.0f, max_x = 0.0f, min_y = 0.0f, max_y = 0.0f, min_z = 0.0f, max_z = 0.0f;
    char bounds3d_seeded = 0;
    const float grid_hi = 1000.0f, grid_lo = -1000.0f;

    surface_worklist[0] = seed_surface_index;

    /* persistent 2D frame for the current surface */
    float origin2d_u = 0.0f, origin2d_v = 0.0f;
    float frame_origin_x = 0.0f, frame_origin_y = 0.0f, frame_origin_z = 0.0f;
    float u_axis_x = 0.0f, u_axis_y = 0.0f, u_axis_z = 0.0f;
    float v_axis_x = 0.0f, v_axis_y = 0.0f, v_axis_z = 0.0f;

    while ( 1 )
    {
        const char *surfaces = (const char *)collision->surfaces.address;
        const char *planes   = (const char *)collision->bsp3d.planes.address;
        int16_t hull_count = 0;
        int surface_index = surface_worklist[worklist_index];
        const int *surface = (const int *)&surfaces[12 * surface_index];
        int current_edge = surface[1];
        const float *plane = (const float *)&planes[16 * *surface];
        float plane_signed[4];   /* indexed by projection axis */
        float pn_x, pn_y, pn_z, pd;
        int projection_axis;
        int proj_sign_slot;
        int keep0, keep1;

        worklist_index = (int16_t)(worklist_index + 1);

        /* signed plane: flip when the surface references the back side of its plane */
        if ( *surface >= 0 )
        {
            pn_x = plane[0]; pn_y = plane[1]; pn_z = plane[2]; pd = plane[3];
        }
        else
        {
            pn_x = -plane[0]; pn_y = -plane[1]; pn_z = -plane[2]; pd = -plane[3];
        }
        plane_signed[0] = pn_x; plane_signed[1] = pn_y; plane_signed[2] = pn_z; plane_signed[3] = pd;

        /* dominant axis of the plane normal (inlined projection_from_vector3d) */
        {
            float ax = (float)__fabs(pn_x), ay = (float)__fabs(pn_y), az = (float)__fabs(pn_z);
            if ( az < ay || az < ax )
                projection_axis = (ay >= ax) ? 1 : 0;
            else
                projection_axis = 2;
        }
        proj_sign_slot = 2 * projection_axis + (plane_signed[projection_axis] > 0.0f);
        keep0 = global_projection3d_mappings[0][proj_sign_slot][0];
        keep1 = global_projection3d_mappings[0][proj_sign_slot][1];

        /* walk this surface's edge loop */
        while ( 1 )
        {
            const collision_edge *edge = &((const collision_edge *)collision->edges.address)[current_edge];
            const char *verts = (const char *)collision->vertices.address;
            int on_surface_b = (surface_index == edge->surface_indices[1]);
            /* pick left/right surface and start/end vertex by which side of the winged edge we are on */
            int neighbor_surface = (&edge->surface_indices[0])[surface_index != edge->surface_indices[1]];
            const float *from_vertex = (const float *)&verts[16 * (&edge->vertex_indices[0])[surface_index != edge->surface_indices[1]]];
            int this_hull = hull_count;

            if ( hull_count != 0 )
            {
                /* subsequent vertices: project onto the established 2D frame, update 2D bbox */
                float u = (from_vertex[0] * u_axis_x + (from_vertex[2] * u_axis_z + from_vertex[1] * u_axis_y)) - origin2d_u;
                float v = (from_vertex[0] * v_axis_x + (from_vertex[2] * v_axis_z + from_vertex[1] * v_axis_y)) - origin2d_v;
                if ( u <= min_u ) min_u = u;
                if ( v <= min_v ) min_v = v;
                if ( u > max_u )  max_u = u;
                if ( v > max_v )  max_v = v;
            }
            else
            {
                /* first edge: establish the 2D frame for this surface */
                const float *to_vertex = (const float *)&verts[16 * (&edge->vertex_indices[0])[on_surface_b]];
                float edge_dx, edge_dy, edge_dz, edge_len;

                if ( surface_index == seeded_index )
                {
                    /* seed surface: origin is the damage epicenter projected onto the plane */
                    float frame[3];
                    frame[keep0] = damage_data->epicenter.n[keep0];
                    frame[keep1] = damage_data->epicenter.n[keep1];
                    if ( __fabs(plane_signed[projection_axis]) >= 0.000099999997f )
                        frame[projection_axis] =
                            -((plane_signed[keep1] * damage_data->epicenter.n[keep1])
                              - (-((plane_signed[keep0] * damage_data->epicenter.n[keep0]) - pd)))
                            / plane_signed[projection_axis];
                    else
                        frame[projection_axis] = 0.0f;
                    frame_origin_x = frame[0]; frame_origin_y = frame[1]; frame_origin_z = frame[2];
                }
                else
                {
                    frame_origin_x = from_vertex[0];
                    frame_origin_y = from_vertex[1];
                    frame_origin_z = from_vertex[2];
                }

                /* u_axis = normalized first-edge direction */
                edge_dx = to_vertex[0] - from_vertex[0];
                edge_dy = to_vertex[1] - from_vertex[1];
                edge_dz = to_vertex[2] - from_vertex[2];
                edge_len = __fsqrts(edge_dx * edge_dx + (edge_dz * edge_dz + edge_dy * edge_dy));
                if ( __fabs(edge_len) >= 0.000099999997f )
                {
                    float inv = 1.0f / edge_len;
                    edge_dx *= inv; edge_dy *= inv; edge_dz *= inv;
                }
                u_axis_x = edge_dx; u_axis_y = edge_dy; u_axis_z = edge_dz;

                /* v_axis = u_axis x plane_normal */
                v_axis_x = edge_dy * pn_z - edge_dz * pn_y;
                v_axis_y = edge_dz * pn_x - pn_z * edge_dx;
                v_axis_z = pn_y * edge_dx - edge_dy * pn_x;

                origin2d_u = frame_origin_x * u_axis_x + (frame_origin_y * u_axis_y + frame_origin_z * u_axis_z);
                origin2d_v = frame_origin_x * v_axis_x + (frame_origin_y * v_axis_y + frame_origin_z * v_axis_z);

                /* seed the 2D bbox with the first ("from") vertex's coords */
                max_u = (from_vertex[0] * u_axis_x + (from_vertex[2] * u_axis_z + from_vertex[1] * u_axis_y)) - origin2d_u;
                min_u = max_u;
                max_v = (from_vertex[0] * v_axis_x + (from_vertex[2] * v_axis_z + from_vertex[1] * v_axis_y)) - origin2d_v;
                min_v = max_v;
            }

            /* record the projected vertex into the 2D hull and update the 3D bbox */
            {
                float u2d = from_vertex[keep0];
                float v2d = from_vertex[keep1];
                hull_points[this_hull].n[0] = u2d;
                hull_points[this_hull].n[1] = v2d;

                if ( bounds3d_seeded )
                {
                    if ( from_vertex[0] <= min_x ) min_x = from_vertex[0];
                    if ( from_vertex[1] <= min_y ) min_y = from_vertex[1];
                    if ( from_vertex[2] <= min_z ) min_z = from_vertex[2];
                    if ( from_vertex[0] > max_x )  max_x = from_vertex[0];
                    if ( from_vertex[1] > max_y )  max_y = from_vertex[1];
                    if ( from_vertex[2] > max_z )  max_z = from_vertex[2];
                }
                else
                {
                    max_x = from_vertex[0]; min_x = max_x;
                    max_y = from_vertex[1]; min_y = max_y;
                    max_z = from_vertex[2]; min_z = max_z;
                    bounds3d_seeded = 1;
                }
            }

            /* enqueue the neighbor surface if it belongs to the same breakable surface + material */
            if ( neighbor_surface != -1 )
            {
                int j;
                for ( j = 0; j < worklist_count; ++j )
                {
                    if ( surface_worklist[j] == neighbor_surface )
                    {
                        neighbor_surface = -1;
                        break;
                    }
                }
                if ( neighbor_surface != -1 )
                {
                    const char *neighbor = &surfaces[12 * neighbor_surface];
                    if ( (unsigned char)neighbor[9] == breakable_surface_index
                         && *(const uint16_t *)&neighbor[10] == (uint16_t)seed_surface->material_index )
                    {
                        surface_worklist[worklist_count] = neighbor_surface;
                        worklist_count = (int16_t)(worklist_count + 1);
                    }
                }
            }

            hull_count = (int16_t)(this_hull + 1);
            current_edge = (&edge->edge_indices[0])[on_surface_b];
            if ( current_edge == surface[1] )
                break;
        }

        /* particle emitters for this surface */
        {
            int emitter_count = effect_block->particle_effects.count;
            int e;
            for ( e = 0; e < emitter_count; ++e )
            {
                const breakable_surface_particle_effect *emitter =
                    &((const breakable_surface_particle_effect *)effect_block->particle_effects.address)[e];
                float spacing;
                int grid_u_lo, grid_u_hi, grid_v_lo, grid_v_hi;

                if ( emitter->particle.index == -1 )
                    continue;

                spacing = emitter->density;
                if ( spacing == 0.0f )
                {
                    /* zero spacing: single cell, or none on the seed surface */
                    grid_v_lo = grid_v_hi = 0;
                    grid_u_lo = grid_u_hi = (surface_index == seeded_index) ? 0 : 1;
                }
                else
                {
                    float a, b;
                    a = min_u / spacing;  a = (a >= grid_lo) ? (a > grid_hi ? grid_hi : a) : grid_lo;
                    grid_u_lo = (int)ceilf(a);
                    b = min_v / spacing;  b = (b >= grid_lo) ? (b > grid_hi ? grid_hi : b) : grid_lo;
                    grid_v_lo = (int)ceilf(b);
                    a = max_u / spacing;  a = (a >= grid_lo) ? (a > grid_hi ? grid_hi : a) : grid_lo;
                    grid_u_hi = (int)floorf(a);
                    b = max_v / spacing;  b = (b >= grid_lo) ? (b > grid_hi ? grid_hi : b) : grid_lo;
                    grid_v_hi = (int)floorf(b);
                }

                if ( grid_v_lo > grid_v_hi )
                    continue;

                {
                    int gv;
                    for ( gv = grid_v_lo; gv <= grid_v_hi; ++gv )
                    {
                        int gu;
                        if ( grid_u_lo > grid_u_hi )
                            continue;
                        for ( gu = grid_u_lo; gu <= grid_u_hi; ++gu )
                        {
                            new_particle_data particle;
                            real_point2d test_point;
                            real_point3d spawn;
                            float frame_pt[3];
                            float jitter_u, jitter_v, cell_size;
                            int kc0 = keep0, kc1 = keep1;

                            spawn.n[0] = frame_origin_x;
                            spawn.n[1] = frame_origin_y;
                            spawn.n[2] = frame_origin_z;

                            jitter_u = real_seed_random_range(get_global_local_random_seed_address(), -0.75f, 0.75f);
                            jitter_v = real_seed_random_range(get_global_local_random_seed_address(), -0.75f, 0.75f);
                            cell_size = emitter->density;

                            /* place the spawn point at grid cell (gu,gv) plus jitter, in world space */
                            spawn.n[1] = v_axis_y * (((float)gv + jitter_v) * cell_size)
                                       + (u_axis_y * (((float)gu + jitter_u) * cell_size) + spawn.n[1]);
                            spawn.n[2] = v_axis_z * (((float)gv + jitter_v) * cell_size)
                                       + (u_axis_z * (((float)gu + jitter_u) * cell_size) + spawn.n[2]);
                            spawn.n[0] = (((float)gv + jitter_v) * cell_size) * v_axis_x
                                       + ((((float)gu + jitter_u) * cell_size) * u_axis_x + spawn.n[0]);

                            frame_pt[0] = spawn.n[0]; frame_pt[1] = spawn.n[1]; frame_pt[2] = spawn.n[2];
                            test_point.n[0] = frame_pt[kc0];
                            test_point.n[1] = frame_pt[kc1];

                            if ( !convex_hull2d_test_point(hull_count, hull_points, &test_point, 0.0f) )
                                continue;

                            /* build the particle: impulse direction = falloff-shaped pull toward epicenter
                             * plus a damage-direction push, then optional random speed */
                            {
                                float dx = spawn.n[0] - damage_data->epicenter.n[0];
                                float dy = spawn.n[1] - damage_data->epicenter.n[1];
                                float dz = spawn.n[2] - damage_data->epicenter.n[2];
                                const damage_effect_definition *damage_def =
                                    TAG_GET(const damage_effect_definition, damage_data->definition_index);
                                real_vector3d impulse = *global_zero_vector3d;
                                float dist = __fsqrts(dx * dx + (dz * dz + dy * dy));

                                if ( __fabs(dist) < 0.000099999997f )
                                {
                                    dist = 0.0f;
                                }
                                else
                                {
                                    float inv = 1.0f / dist;
                                    dx *= inv; dy *= inv; dz *= inv;
                                }

                                /* radial pull, scaled by the definition's outward (vacuum) falloff */
                                if ( damage_def->breaking_effect.outward_radius > 0.0f )
                                {
                                    float t = 1.0f - dist / damage_def->breaking_effect.outward_radius;
                                    t = (t >= 0.0f) ? (t > 1.0f ? 1.0f : t) : 0.0f;
                                    if ( damage_def->breaking_effect.outward_exponent != 0.0f )
                                        t = (float)pow(t, damage_def->breaking_effect.outward_exponent);
                                    impulse.n[0] = damage_def->breaking_effect.outward_velocity * t * dx + impulse.n[0];
                                    impulse.n[1] = dy * (damage_def->breaking_effect.outward_velocity * t) + impulse.n[1];
                                    impulse.n[2] = dz * (damage_def->breaking_effect.outward_velocity * t) + impulse.n[2];
                                }

                                /* directional push along the damage direction (forward falloff) */
                                if ( damage_def->breaking_effect.forward_radius > 0.0f )
                                {
                                    float t = 1.0f - dist / damage_def->breaking_effect.forward_radius;
                                    t = (t >= 0.0f) ? (t > 1.0f ? 1.0f : t) : 0.0f;
                                    /* deviation: original reuses the outward_exponent here (not forward_exponent);
                                     * forward_exponent only gates whether the pow is applied. */
                                    if ( damage_def->breaking_effect.forward_exponent != 0.0f )
                                        t = (float)pow(t, damage_def->breaking_effect.outward_exponent);
                                    impulse.n[0] = damage_def->breaking_effect.forward_velocity * t * damage_data->direction.n[0] + impulse.n[0];
                                    impulse.n[1] = damage_data->direction.n[1] * (damage_def->breaking_effect.forward_velocity * t) + impulse.n[1];
                                    impulse.n[2] = damage_data->direction.n[2] * (damage_def->breaking_effect.forward_velocity * t) + impulse.n[2];
                                }

                                /* optional random velocity scale (emitter +24/+28) */
                                if ( emitter->velocity_scale_upper_bound > 0.0f )
                                {
                                    float s = real_seed_random_range(get_global_local_random_seed_address(),
                                            emitter->velocity_scale_lower_bound, emitter->velocity_scale_upper_bound);
                                    impulse.n[0] = s * impulse.n[0];
                                    impulse.n[1] = impulse.n[1] * s;
                                    impulse.n[2] = impulse.n[2] * s;
                                }

                                particle.dont_draw_first_person = 0;
                                particle.direction = impulse;
                                particle.velocity = impulse;
                                particle.definition_index = emitter->particle.index;
                                particle.position = spawn;
                                particle.attached_to_local_player = 0;
                                particle.dont_draw_third_person = 0;
                                particle.node_index = -1;
                                particle.object_index = -1;
                                particle.local_player_index = -1;
                                particle.initial_impulse = *global_zero_vector3d;
                                particle.rotation = real_seed_random_range(get_global_local_random_seed_address(), 0.0f, TWO_PI);
                                particle.angular_velocity = real_seed_random_range(get_global_local_random_seed_address(),
                                        emitter->angular_velocity_lower_bound, emitter->angular_velocity_upper_bound);
                                particle.radius = real_seed_random_range(get_global_local_random_seed_address(),
                                        emitter->radius_lower_bound, emitter->radius_upper_bound);
                                rgb_colors_interpolate(&particle.color.__s1.rgb,
                                        /* low 2 bits = color-interpolate flags (DB enum $ACEEECA7...) */
                                        emitter->flags & ((1u << _breakable_surface_particle_effect_color_interpolate_in_hsv_bit)
                                                        | (1u << _breakable_surface_particle_effect_color_interpolate_along_farthest_hue_path_bit)),
                                        (const real_rgb_color *)&emitter->tint_lower_bound.n[1],
                                        (const real_rgb_color *)&emitter->tint_upper_bound.n[1],
                                        real_seed_random(get_global_local_random_seed_address()));

                                /* alpha/intensity (emitter +68 low, +84 high), clamped to [0,1].
                                 * deviation: faithful to the original, the random range is re-drawn for
                                 * each of the three uses (>=0 test, <=1 test, and the stored value). */
                                {
                                    float lo = emitter->tint_lower_bound.n[0];
                                    float hi = emitter->tint_upper_bound.n[0];
                                    if ( (hi - lo) * real_seed_random(get_global_local_random_seed_address()) + lo >= 0.0f )
                                    {
                                        if ( (hi - lo) * real_seed_random(get_global_local_random_seed_address()) + lo <= 1.0f )
                                            particle.color.n[0] =
                                                (hi - lo) * real_seed_random(get_global_local_random_seed_address()) + lo;
                                        else
                                            particle.color.n[0] = 1.0f;
                                    }
                                    else
                                    {
                                        particle.color.n[0] = 0.0f;
                                    }
                                }

                                /* normalize the particle direction, or pick a random one if degenerate */
                                {
                                    float len = __fsqrts(particle.direction.n[0] * particle.direction.n[0]
                                            + (particle.direction.n[1] * particle.direction.n[1]
                                               + particle.direction.n[2] * particle.direction.n[2]));
                                    int degenerate = __fabs(len) < 0.000099999997f;
                                    if ( !degenerate )
                                    {
                                        float inv = 1.0f / len;
                                        particle.direction.n[0] = inv * particle.direction.n[0];
                                        particle.direction.n[1] = particle.direction.n[1] * inv;
                                        particle.direction.n[2] = particle.direction.n[2] * inv;
                                        degenerate = (len == 0.0f);
                                    }
                                    if ( degenerate )
                                        seed_random_direction3d(get_global_local_random_seed_address(),
                                                &particle.direction);
                                }

                                particle_new(&particle);
                            }
                        }
                    }
                }
            }
        }

        if ( worklist_index >= worklist_count )
            break;
    }

    /* impulse break sound at the center of the group's 3D bounds */
    if ( effect_block->sound.index != -1 )
    {
        sound_location sound_loc;
        sound_loc.game_location = damage_data->location;
        sound_loc.position.n[0] = (max_x + min_x) * 0.5f;
        sound_loc.position.n[1] = (max_y + min_y) * 0.5f;
        sound_loc.position.n[2] = (max_z + min_z) * 0.5f;
        sound_loc.forward.n[0] = global_forward3d->n[0];
        sound_loc.forward.n[1] = global_forward3d->n[1];
        sound_loc.forward.n[2] = global_forward3d->n[2];
        sound_loc.translational_velocity.n[0] = global_zero_vector3d->n[0];
        sound_loc.translational_velocity.n[1] = global_zero_vector3d->n[1];
        sound_loc.translational_velocity.n[2] = global_zero_vector3d->n[2];
        unattached_impulse_sound_new(effect_block->sound.index, &sound_loc, 1.0f, 0);
    }
    }
}
