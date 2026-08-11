/* decal_new_from_collision @0x83741948 — spawn a decal (and its whole next_decal_in_chain chain) at a collision
 * point: build the projection basis from the surface normal (optionally velocity-aligned or randomly
 * rotated), clip it onto the touched BSP surfaces, wrap it across the "deviant" surfaces whose angle
 * exceeds the decal type's minimum_wrap_angle, then emit the clipped triangle fans into a freshly
 * allocated rasterizer decal vertex cache and register the decal datum.
 *
 * The Hex-Rays decompiler fails on this function ("call analysis failed"); this is a hand
 * reconstruction from the disassembly at 0x83741948-0x83742E88. Deviations from a naive reading are
 * marked inline. Register-allocated stack-slot reuse in the binary is split back into the distinct
 * source-level locals it came from. */

#include <stdint.h>
#include "headers/collision_result.h"
#include "headers/collision_bsp.h"
#include "headers/decal_definition.h"
#include "headers/decal_datum.h"
#include "headers/decal_geometry.h"
#include "headers/decal_projection.h"
#include "headers/decal_render_vertex.h"
#include "headers/decal_editor_geometry.h"
#include "headers/decal_wrap_parameters.h"
#include "headers/bitmap_group.h"
#include "headers/bitmap_group_sequence.h"
#include "headers/bitmap_group_sprite.h"
#include "headers/bitmap_data.h"
#include "headers/real_vector3d.h"
#include "headers/real_point3d.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_rectangle2d.h"
#include "headers/real_rgb_color.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/decal_definition_flags.h"
#include "headers/bitmap_group_type.h"
#include "headers/math_constants.h"
#include "headers/blam_data_globals.h"


extern void  hcex_init_decal(int decal_definition_index, const real_point3d *point);
extern int16_t projection_from_vector3d(const real_vector3d *n);
extern float normalize3d(real_vector3d *v);
extern real_vector3d *perpendicular3d(const real_vector3d *a, real_vector3d *result);
extern float angle_between_normals3d(const real_vector3d *a, const real_vector3d *b);
extern void matrix4x3_rotation_from_axis_and_angle(real_matrix4x3 *matrix, const real_vector3d *axis, float sine, float cosine);
extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);
extern real_vector3d *matrix4x3_transform_normal(const real_matrix4x3 *matrix, const real_vector3d *normal, real_vector3d *result);
extern uint32_t *get_global_local_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);
extern int16_t seed_random_range(uint32_t *seed, int16_t lower_bound, int16_t upper_bound);
extern void decal_sprite_get_bounds(const decal_definition *definition, int16_t sequence_index, int16_t sprite_index, float radius, real_rectangle2d *sprite_bounds, real_rectangle2d *extent);
extern void decal_projection_create(const real_matrix4x3 *basis, const real_rectangle2d *extent, decal_projection *projection);
extern void decal_clip_to_surface(const decal_projection *projection, decal_geometry *geometry, int surface_index, uint8_t update_queue, float radius, int16_t type, int *surface_queue, int16_t *surface_queue_write_index, int *deviant_surface_list, int16_t *deviant_surface_count);
extern int _texture_cache_bitmap_get_hardware_format(bitmap_data *bitmap, uint8_t block, uint8_t load);
extern int rasterizer_decal_vertices_new(int cache_size);
extern int decal_insert(int cache_index, int16_t cluster_index, int16_t layer, int next_decal_index, uint8_t permanent);
extern void *datum_get(data_array *data, int index);
extern void *rasterizer_decal_vertices_lock(int cache_index, int cache_size);
extern void rasterizer_decal_vertices_unlock(void);
extern void rasterizer_decal_vertices_delete(int cache_index);
extern real_rgb_color *rgb_colors_interpolate(real_rgb_color *rgb_result, unsigned int flags, const real_rgb_color *rgb_lower_bound, const real_rgb_color *rgb_upper_bound, float u);
extern int game_time_get(void);
extern double cos(double x);
extern double sin(double x);
extern double floor(double x);
extern float sqrtf(float x);

/* Resolve a plane designator into a plane, negating it when bit 31 marks the surface back-facing (0x83742230-
 * 0x83742270 idiom). DEVIATION: `slwi r9,r4,4` @0x83742230 discards bit 31, so the mask is an x64-only no-op. */
static void resolve_surface_plane(const collision_bsp *bsp, int plane_designator, real_plane3d *out_plane)
{
    const real_plane3d *plane = &((const real_plane3d *)bsp->bsp3d.planes.address)[plane_designator & 0x7FFFFFFF];
    if (plane_designator < 0)
    {
        out_plane->normal.i = -plane->normal.i;
        out_plane->normal.j = -plane->normal.j;
        out_plane->normal.k = -plane->normal.k;
        out_plane->distance = -plane->distance;
    }
    else
    {
        *out_plane = *plane;
    }
}

void decal_new_from_collision(int decal_definition_index, const collision_result *collision,
    const real_vector3d *velocity, float radius_modifier, uint8_t permanent,
    int16_t forced_sequence_index, decal_editor_geometry *editor_geometry)
{
    const float k_epsilon = 1e-4f;            /* __real_38d1b717 */
    const float k_neg_epsilon = -1e-4f;       /* __real_b8d1b717 */
    const float k_two_pi = TWO_PI;        /* __real_40c90fdb */
    const float k_255 = 255.0f;               /* __real_437f0000 */
    const float k_254 = 254.0f;               /* __real_437e0000 */
    const float k_half = 0.5f;                /* __real_3f000000 */
    const float k_depth_bias = 0.00390625f;   /* __real_3b800000 = 1/256 */

    collision_bsp *bsp = global_collision_bsp; /* snapshot at entry (arg_108) */

    /* decal-chain persistent state (survives across chain iterations, as stack slots do in the binary) */
    real_vector3d decal_forward, decal_left, decal_up;   /* projection basis rows (arg_144/150/15C) */
    real_point3d  decal_position;                        /* arg_168 */
    real_rectangle2d projection_extent;                  /* arg_130 — sprite bounds fed to projection_create */
    real_rectangle2d texcoord_extent;                    /* arg_E0  — texture-space rect for the vertices */
    float radius = 0.0f;                                 /* arg_8C */
    int16_t sequence_index = 0;                          /* arg_74 / r18 */
    int16_t decal_bitmap_index = 0;                      /* r19 */
    unsigned char chain_reuses_basis = 0;                /* r31 low byte at loop top */

    (void)editor_geometry; /* passed through by decal_new; unused in this build */

    hcex_init_decal(decal_definition_index, &collision->point);

    if (decal_definition_index == -1)
        return;

    if (radius_modifier == 0.0f)          /* 0x83741F84 — a zero modifier means "1x" */
        radius_modifier = 1.0f;

    for (int current_definition_index = decal_definition_index;
         current_definition_index != -1; )
    {
        decal_definition *definition =
            TAG_GET(decal_definition, current_definition_index);
        bitmap_group *bitmap_group_def = TAG_GET(bitmap_group, definition->shader.decal.map.index); /* 0xE4(def) */

        /* ---- Phase A: build the decal projection basis (skipped when a chained decal reuses it) ---- */
        if (!chain_reuses_basis && (definition->flags & (1u << _decal_definition_no_random_rotation_bit)))
        {
            real_vector3d normal = collision->plane.normal;
            float dot_normal_velocity =
                normal.i * velocity->i + normal.j * velocity->j + normal.k * velocity->k;

            real_vector3d tangent1, tangent2;
            float rotation_cosine = -1.0f;  /* f25 default (non-random branches flip the basis 180deg) */
            float rotation_sine = 0.0f;     /* f24 */
            int basis_ready = 0;

            if (dot_normal_velocity < k_neg_epsilon)  /* velocity heads into the surface */
            {
                real_vector3d seed_vector;

                if (definition->flags & (1u << _decal_definition_SAPIEN_ONLY_snap_to_axis_bit))
                {
                    /* Snap velocity to its dominant axis, then build a tangent frame (0x83741B14). */
                    int16_t axis = projection_from_vector3d(velocity);
                    float sign = (velocity->n[axis] > 0.0f) ? 1.0f : -1.0f;
                    real_vector3d axis_unit = { { 0.0f, 0.0f, 0.0f } };
                    axis_unit.n[axis] = sign;

                    if (dot_normal_velocity > 0.0f)
                    {
                        seed_vector.i = normal.i + axis_unit.i;
                        seed_vector.j = normal.j + axis_unit.j;
                        seed_vector.k = normal.k + axis_unit.k;
                    }
                    else
                    {
                        seed_vector.i = axis_unit.i - normal.i;
                        seed_vector.j = axis_unit.j - normal.j;
                        seed_vector.k = axis_unit.k - normal.k;
                    }
                    normalize3d(&seed_vector);

                    /* tangent1 = seed_vector x normal, tangent2 = normal x tangent1 */
                    tangent1.i = seed_vector.k * normal.j - seed_vector.j * normal.k;
                    tangent1.j = seed_vector.i * normal.k - seed_vector.k * normal.i;
                    tangent1.k = seed_vector.j * normal.i - seed_vector.i * normal.j;
                    tangent2.i = normal.j * tangent1.k - normal.k * tangent1.j;
                    tangent2.j = normal.k * tangent1.i - normal.i * tangent1.k;
                    tangent2.k = normal.i * tangent1.j - normal.j * tangent1.i;

                    float tangent1_len_sq =
                        tangent1.i * tangent1.i + tangent1.j * tangent1.j + tangent1.k * tangent1.k;
                    float tangent2_len_sq =
                        tangent2.i * tangent2.i + tangent2.j * tangent2.j + tangent2.k * tangent2.k;

                    if (tangent1_len_sq >= k_epsilon && tangent2_len_sq >= k_epsilon)
                        basis_ready = 1;
                    else
                    {
                        /* Degenerate: rebuild off the velocity projected onto the surface plane (0x83741C44). */
                        real_vector3d projected;
                        projected.i = velocity->i - normal.i * dot_normal_velocity;
                        projected.j = velocity->j - normal.j * dot_normal_velocity;
                        projected.k = velocity->k - normal.k * dot_normal_velocity;

                        int16_t axis2 = projection_from_vector3d(&projected);
                        float sign2 = (projected.n[axis2] > 0.0f) ? 1.0f : -1.0f;
                        real_vector3d axis_unit2 = { { 0.0f, 0.0f, 0.0f } };
                        axis_unit2.n[axis2] = sign2;

                        if (/* dot(projected-derived) */ (axis_unit2.i * normal.i + axis_unit2.j * normal.j + axis_unit2.k * normal.k) > 0.0f)
                        {
                            seed_vector.i = normal.i + axis_unit2.i;
                            seed_vector.j = normal.j + axis_unit2.j;
                            seed_vector.k = normal.k + axis_unit2.k;
                        }
                        else
                        {
                            seed_vector.i = axis_unit2.i - normal.i;
                            seed_vector.j = axis_unit2.j - normal.j;
                            seed_vector.k = axis_unit2.k - normal.k;
                        }
                        normalize3d(&seed_vector);

                        tangent1.i = seed_vector.k * normal.j - seed_vector.j * normal.k;
                        tangent1.j = seed_vector.i * normal.k - seed_vector.k * normal.i;
                        tangent1.k = seed_vector.j * normal.i - seed_vector.i * normal.j;
                        tangent2.i = normal.j * tangent1.k - normal.k * tangent1.j;
                        tangent2.j = normal.k * tangent1.i - normal.i * tangent1.k;
                        tangent2.k = normal.i * tangent1.j - normal.j * tangent1.i;
                        basis_ready = 1;
                    }
                }
                else
                {
                    /* No dominant-axis snap: tangent frame straight from normal x velocity (0x83741D90). */
                    tangent1.i = normal.j * velocity->k - normal.k * velocity->j;
                    tangent1.j = normal.k * velocity->i - normal.i * velocity->k;
                    tangent1.k = normal.i * velocity->j - normal.j * velocity->i;
                    tangent2.i = normal.j * tangent1.k - normal.k * tangent1.j;
                    tangent2.j = normal.k * tangent1.i - normal.i * tangent1.k;
                    tangent2.k = normal.i * tangent1.j - normal.j * tangent1.i;
                    basis_ready = 1;
                }
            }

            if (!basis_ready)
            {
                /* Random orientation about the surface normal (0x83741DE8). */
                unsigned int *seed = get_global_local_random_seed_address();
                float angle = real_seed_random_range(seed, 0.0f, k_two_pi);
                rotation_cosine = (float)cos(angle);
                rotation_sine = (float)sin(angle);

                perpendicular3d(&normal, &tangent1);
                tangent2.i = normal.j * tangent1.k - normal.k * tangent1.j;
                tangent2.j = normal.k * tangent1.i - normal.i * tangent1.k;
                tangent2.k = normal.i * tangent1.j - normal.j * tangent1.i;
            }

            /* Common tail (0x83741E50): normalize both tangents, rotate them into the U/V axes. */
            {
                float len1 = tangent1.i * tangent1.i + tangent1.j * tangent1.j + tangent1.k * tangent1.k;
                if (len1 >= k_epsilon)  /* fsqrts then fabs compared to epsilon */
                {
                    float inv = 1.0f / sqrtf(len1);
                    tangent1.i *= inv; tangent1.j *= inv; tangent1.k *= inv;
                }
                float len2 = tangent2.i * tangent2.i + tangent2.j * tangent2.j + tangent2.k * tangent2.k;
                if (len2 >= k_epsilon)
                {
                    float inv = 1.0f / sqrtf(len2);
                    tangent2.i *= inv; tangent2.j *= inv; tangent2.k *= inv;
                }
            }

            decal_forward.i = tangent2.i * rotation_cosine - tangent1.i * rotation_sine;
            decal_forward.j = tangent2.j * rotation_cosine - tangent1.j * rotation_sine;
            decal_forward.k = tangent2.k * rotation_cosine - tangent1.k * rotation_sine;
            decal_left.i = tangent1.i * rotation_cosine + tangent2.i * rotation_sine;
            decal_left.j = tangent1.j * rotation_cosine + tangent2.j * rotation_sine;
            decal_left.k = tangent1.k * rotation_cosine + tangent2.k * rotation_sine;
            decal_up = normal;
            decal_position = collision->point;
        }
        else if (!chain_reuses_basis)
        {
            /* definition->flags bit3 clear: random orientation about the surface normal (0x83741DE8). */
            real_vector3d normal = collision->plane.normal;
            real_vector3d tangent1, tangent2;
            unsigned int *seed = get_global_local_random_seed_address();
            float angle = real_seed_random_range(seed, 0.0f, k_two_pi);
            float rotation_cosine = (float)cos(angle);
            float rotation_sine = (float)sin(angle);

            perpendicular3d(&normal, &tangent1);
            tangent2.i = normal.j * tangent1.k - normal.k * tangent1.j;
            tangent2.j = normal.k * tangent1.i - normal.i * tangent1.k;
            tangent2.k = normal.i * tangent1.j - normal.j * tangent1.i;

            float len1 = tangent1.i * tangent1.i + tangent1.j * tangent1.j + tangent1.k * tangent1.k;
            if (len1 >= k_epsilon)
            {
                float inv = 1.0f / sqrtf(len1);
                tangent1.i *= inv; tangent1.j *= inv; tangent1.k *= inv;
            }
            float len2 = tangent2.i * tangent2.i + tangent2.j * tangent2.j + tangent2.k * tangent2.k;
            if (len2 >= k_epsilon)
            {
                float inv = 1.0f / sqrtf(len2);
                tangent2.i *= inv; tangent2.j *= inv; tangent2.k *= inv;
            }

            decal_forward.i = tangent2.i * rotation_cosine - tangent1.i * rotation_sine;
            decal_forward.j = tangent2.j * rotation_cosine - tangent1.j * rotation_sine;
            decal_forward.k = tangent2.k * rotation_cosine - tangent1.k * rotation_sine;
            decal_left.i = tangent1.i * rotation_cosine + tangent2.i * rotation_sine;
            decal_left.j = tangent1.j * rotation_cosine + tangent2.j * rotation_sine;
            decal_left.k = tangent1.k * rotation_cosine + tangent2.k * rotation_sine;
            decal_up = normal;
            decal_position = collision->point;
        }

        /* ---- sequence + radius (also skipped when a chained decal reuses the basis, 0x83741F38) ---- */
        if (!chain_reuses_basis)
        {
            if (forced_sequence_index != -1)
            {
                sequence_index = forced_sequence_index;
            }
            else
            {
                unsigned int *seed = get_global_local_random_seed_address();
                int16_t count = (int16_t)bitmap_group_def->sequences.count;
                sequence_index = seed_random_range(seed, 0, count);
                if (sequence_index >= count)
                    sequence_index = count - 1;
            }

            {
                unsigned int *seed = get_global_local_random_seed_address();
                radius = real_seed_random_range(seed, definition->radius_lower_bounds,
                                                definition->radius_upper_bounds) * radius_modifier;
            }
        }

        /* ---- sprite bounds / texture rect (0x83741FBC) ---- */
        if (bitmap_group_def->type == _bitmap_group_type_sprites)
        {
            /* sprite-sheet: resolve the first sprite of the chosen sequence for its bitmap index */
            const bitmap_group_sequence *sequence =
                (const bitmap_group_sequence *)bitmap_group_def->sequences.address + sequence_index;
            const bitmap_group_sprite *sprite =
                (const bitmap_group_sprite *)sequence->sprites.address; /* sprite_index 0 */
            decal_bitmap_index = sprite->bitmap_index;
            decal_sprite_get_bounds(definition, sequence_index, 0, radius,
                                    &projection_extent, &texcoord_extent);
        }
        else
        {
            /* single bitmap: build bounds from radius (and optional height/width aspect, flag 0x100) */
            float aspect = 1.0f;
            decal_bitmap_index = 0;
            if (definition->flags & (1u << _decal_definition_preserve_aspect_bit))
            {
                const bitmap_data *first = (const bitmap_data *)bitmap_group_def->bitmaps.address;
                aspect = (float)first->height / (float)first->width;
            }
            aspect *= radius;
            projection_extent.__s1.x0 = -radius;
            projection_extent.__s1.x1 = radius;
            projection_extent.__s1.y0 = -aspect;
            projection_extent.__s1.y1 = aspect;
            texcoord_extent.__s1.x0 = 0.0f;
            texcoord_extent.__s1.x1 = 1.0f;
            texcoord_extent.__s1.y0 = 0.0f;
            texcoord_extent.__s1.y1 = 1.0f;
        }

        /* ---- non-permanent decals must have their bitmap resident before drawing (0x83742094) ---- */
        if (!permanent && !debug_no_drawing)
        {
            bitmap_data *bitmaps = (bitmap_data *)bitmap_group_def->bitmaps.address;
            if (!_texture_cache_bitmap_get_hardware_format(&bitmaps[decal_bitmap_index], 0, 1))
                return;
        }

        /* ---- clip the decal onto the surfaces it touches ---- */
        decal_geometry geometry;
        geometry.decal_vertex_count = 0;
        geometry.decal_surface_count = 0;

        real_matrix4x3 basis;
        basis.scale = 1.0f;   /* DEVIATION: the scale slot (arg_140) is not stored within this function's
                                 range; it mirrors the second projection's basis (arg_1A0 = 1.0). */
        basis.forward = decal_forward;
        basis.left = decal_left;
        basis.up = decal_up;
        basis.position = decal_position;

        decal_projection projection;
        decal_projection_create(&basis, &projection_extent, &projection);

        int surface_queue[1024];
        int16_t surface_queue_count = 1;
        int deviant_surface_list[1024];
        int16_t deviant_surface_count = 0;
        surface_queue[0] = collision->surface_index;

        for (int queue_index = 0; queue_index < surface_queue_count; queue_index++)
        {
            decal_clip_to_surface(&projection, &geometry, surface_queue[queue_index], 1, radius,
                                  definition->type, surface_queue, &surface_queue_count,
                                  deviant_surface_list, &deviant_surface_count);
        }

        /* extent accumulators (arg_25/20/…) track the wrapped up-axis across every projection */
        float min_up_x = decal_up.i, max_up_x = decal_up.i;
        float min_up_y = decal_up.j, max_up_y = decal_up.j;
        float min_up_z = decal_up.k, max_up_z = decal_up.k;

        /* ---- wrap the decal across deviant surfaces (0x837421A8) ---- */
        int owner_group[1024];
        if (decal_wrap_parameters[definition->type].wrap && deviant_surface_count > 0)
        {
            real_plane3d projection_plane = projection.plane;
            int remaining_deviant = deviant_surface_count;

            for (int i = 0; i < deviant_surface_count && remaining_deviant > 0; i++)
            {
                if (deviant_surface_list[i] == -1)
                    continue;

                /* start a new wrap group owned by surface deviant_surface_list[i] */
                real_plane3d owner_plane;
                resolve_surface_plane(bsp, ((const collision_surface *)bsp->surfaces.address)[deviant_surface_list[i]].plane_designator, &owner_plane);

                int group_size = 0;
                owner_group[group_size++] = deviant_surface_list[i];
                deviant_surface_list[i] = -1;

                for (int j = i + 1; j < deviant_surface_count; j++)
                {
                    if (deviant_surface_list[j] == -1)
                        continue;

                    real_plane3d neighbor_plane;
                    resolve_surface_plane(bsp, ((const collision_surface *)bsp->surfaces.address)[deviant_surface_list[j]].plane_designator, &neighbor_plane);

                    float angle = angle_between_normals3d(&owner_plane.normal, &neighbor_plane.normal);
                    if (angle > decal_wrap_parameters[definition->type].minimum_wrap_angle * DEG_TO_RAD)
                        continue;

                    owner_group[group_size++] = deviant_surface_list[j];
                    deviant_surface_list[j] = -1;
                }

                /* ---- find the seam edge for this group and build the wrapped projection ---- */
                real_plane3d seam_surface_plane;
                real_point3d seam_start = { { 0, 0, 0 } };  /* arg_B0 = v_b */
                real_point3d seam_end = { { 0, 0, 0 } };     /* arg_110 = v_a */
                int current_owner = -1;
                float best_min_distance = 0.0f;
                float best_max_distance = 0.0f;

                for (int m = 0; m < group_size; m++)
                {
                    int surface_index = owner_group[m];
                    const collision_surface *surface =
                        &((const collision_surface *)bsp->surfaces.address)[surface_index];
                    int edge_index = surface->first_edge_index;

                    do
                    {
                        const collision_edge *edge =
                            &((const collision_edge *)bsp->edges.address)[edge_index];
                        int is_right = (surface_index == edge->surface_indices[1]);
                        int vertex_a = is_right ? edge->vertex_indices[1] : edge->vertex_indices[0];
                        int vertex_b = is_right ? edge->vertex_indices[0] : edge->vertex_indices[1];

                        const collision_vertex *va =
                            &((const collision_vertex *)bsp->vertices.address)[vertex_a];
                        const collision_vertex *vb =
                            &((const collision_vertex *)bsp->vertices.address)[vertex_b];

                        float dist_a = projection_plane.normal.i * va->point.x +
                                       projection_plane.normal.j * va->point.y +
                                       projection_plane.normal.k * va->point.z - projection_plane.distance;
                        float dist_b = projection_plane.normal.i * vb->point.x +
                                       projection_plane.normal.j * vb->point.y +
                                       projection_plane.normal.k * vb->point.z - projection_plane.distance;
                        dist_a = dist_a < 0.0f ? -dist_a : dist_a;
                        dist_b = dist_b < 0.0f ? -dist_b : dist_b;
                        float min_distance = dist_a < dist_b ? dist_a : dist_b;
                        float max_distance = dist_a < dist_b ? dist_b : dist_a;

                        if (current_owner == -1 ||
                            !(min_distance > best_min_distance || max_distance > best_max_distance))
                        {
                            resolve_surface_plane(bsp, surface->plane_designator, &seam_surface_plane);
                            best_min_distance = min_distance;
                            best_max_distance = max_distance;
                            current_owner = surface_index;
                            seam_start = vb->point;
                            seam_end = va->point;
                        }

                        edge_index = is_right ? edge->edge_indices[1] : edge->edge_indices[0];
                    } while (edge_index != surface->first_edge_index);
                }

                decal_projection wrapped_projection;
                real_vector3d seam_direction;
                seam_direction.i = seam_end.x - seam_start.x;
                seam_direction.j = seam_end.y - seam_start.y;
                seam_direction.k = seam_end.z - seam_start.z;
                float seam_length = sqrtf(
                    seam_direction.i * seam_direction.i +
                    seam_direction.j * seam_direction.j +
                    seam_direction.k * seam_direction.k);

                if (seam_length < k_epsilon || seam_length <= 0.0f)
                {
                    /* no usable seam: reuse the base projection unchanged (0x83742790) */
                    wrapped_projection = projection;
                }
                else
                {
                    float inv = 1.0f / seam_length;
                    seam_direction.i *= inv;
                    seam_direction.j *= inv;
                    seam_direction.k *= inv;

                    /* signed wrap angle between the seam surface plane and the projection plane */
                    float orientation =
                        (projection_plane.normal.j * seam_surface_plane.normal.i -
                         projection_plane.normal.i * seam_surface_plane.normal.j) * seam_direction.k +
                        (projection_plane.normal.i * seam_surface_plane.normal.k -
                         projection_plane.normal.k * seam_surface_plane.normal.i) * seam_direction.j +
                        (projection_plane.normal.k * seam_surface_plane.normal.j -
                         projection_plane.normal.j * seam_surface_plane.normal.k) * seam_direction.i;
                    float sign = orientation >= 0.0f ? -1.0f : 1.0f;   /* fsel f31, triple, -1.0, 1.0 */
                    float angle = angle_between_normals3d(&seam_surface_plane.normal, &projection_plane.normal) * sign;
                    float cosine = (float)cos(angle);
                    float sine = (float)sin(angle);

                    real_matrix4x3 wrap_matrix;
                    matrix4x3_rotation_from_axis_and_angle(&wrap_matrix, &seam_direction, sine, cosine);

                    real_point3d point_relative;
                    point_relative.x = decal_position.x - seam_start.x;
                    point_relative.y = decal_position.y - seam_start.y;
                    point_relative.z = decal_position.z - seam_start.z;

                    real_matrix4x3 wrapped_basis;
                    wrapped_basis.scale = 1.0f;
                    matrix4x3_transform_point(&wrap_matrix, &point_relative, &wrapped_basis.position);
                    matrix4x3_transform_normal(&wrap_matrix, &decal_forward, &wrapped_basis.forward);
                    matrix4x3_transform_normal(&wrap_matrix, &decal_left, &wrapped_basis.left);
                    matrix4x3_transform_normal(&wrap_matrix, &decal_up, &wrapped_basis.up);
                    wrapped_basis.position.x += seam_start.x;
                    wrapped_basis.position.y += seam_start.y;
                    wrapped_basis.position.z += seam_start.z;

                    decal_projection_create(&wrapped_basis, &projection_extent, &wrapped_projection);

                    /* expand the up-axis accumulators with the wrapped up-axis (0x83742728) */
                    if (wrapped_basis.up.i < min_up_x) min_up_x = wrapped_basis.up.i;
                    if (wrapped_basis.up.i > max_up_x) max_up_x = wrapped_basis.up.i;
                    if (wrapped_basis.up.j < min_up_y) min_up_y = wrapped_basis.up.j;
                    if (wrapped_basis.up.j > max_up_y) max_up_y = wrapped_basis.up.j;
                    if (wrapped_basis.up.k < min_up_z) min_up_z = wrapped_basis.up.k;
                    if (wrapped_basis.up.k > max_up_z) max_up_z = wrapped_basis.up.k;
                }

                /* clip the wrapped projection onto every surface in the group (no queue growth) */
                for (int n = 0; n < group_size; n++)
                {
                    decal_clip_to_surface(&wrapped_projection, &geometry, owner_group[n], 0, radius,
                                          definition->type, 0, 0, 0, 0);
                }

                remaining_deviant -= group_size;
            }
        }

        /* ---- emit the clipped geometry, if any survived ---- */
        int16_t surface_count = geometry.decal_surface_count;
        if (surface_count <= 0)
            return;
        int16_t vertex_count = geometry.decal_vertex_count;
        if (vertex_count <= 0)
            return;

        /* small decals get nudged toward the averaged up-axis to fight z-fighting (0x83742864) */
        real_vector3d depth_offset = { { 0.0f, 0.0f, 0.0f } };
        if (max_up_x - min_up_x <= k_half &&
            max_up_y - min_up_y <= k_half &&
            max_up_z - min_up_z <= k_half)
        {
            real_vector3d averaged;
            averaged.i = min_up_x + max_up_x;
            averaged.j = min_up_y + max_up_y;
            averaged.k = min_up_z + max_up_z;
            normalize3d(&averaged);
            depth_offset.i = averaged.i * k_depth_bias;
            depth_offset.j = averaged.j * k_depth_bias;
            depth_offset.k = averaged.k * k_depth_bias;
        }

        /* one quad per two fan triangles: floor((vertex_count_of_surface - 1) / 2), summed */
        int16_t quad_count = 0;
        for (int s = 0; s < surface_count; s++)
            quad_count += (geometry.decal_surface_vertex_counts[s] - 1) >> 1;

        int cache_size = quad_count << 6;
        int cache_index = rasterizer_decal_vertices_new(cache_size);
        if (cache_index == -1)
            return;

        int decal_index = decal_insert(cache_index, collision->location.cluster_index,
                                       definition->layer, -1, permanent);
        if (decal_index == -1)
        {
            rasterizer_decal_vertices_delete(cache_index);
            return;
        }

        /* recovered: global_decal_data->data + 56*(u16)decal_index -> datum_get (56 == decal datum_size;
           decal_index just allocated by decal_insert and checked != -1, so the handle is strong) */
        decal_datum *decal = datum_get(global_decal_data, decal_index);
        decal_render_vertex *locked =
            rasterizer_decal_vertices_lock(cache_index, cache_size);
        if (!locked)
        {
            rasterizer_decal_vertices_delete(cache_index);
            return;
        }

        /* build a flat scratch of screen vertices from the clipped geometry */
        decal_render_vertex scratch[1024];
        float u_width = texcoord_extent.__s1.x1 - texcoord_extent.__s1.x0;
        float v_height = texcoord_extent.__s1.y1 - texcoord_extent.__s1.y0;
        for (int v = 0; v < vertex_count; v++)
        {
            const decal_vertex_internal *source = &geometry.decal_vertices[v];

            float u01 = source->texcoord.n[0] * u_width + texcoord_extent.__s1.x0;
            u01 = u01 < 0.0f ? 0.0f : (u01 > 1.0f ? 1.0f : u01);
            float u_scaled = u01 * k_255;
            u_scaled = u_scaled < 0.0f ? 0.0f : (u_scaled > k_254 ? k_254 : u_scaled);
            int u_texel = (int)floor(u_scaled + k_half);

            float v_texcoord01 = source->texcoord.n[1] * v_height + texcoord_extent.__s1.y0;
            v_texcoord01 = v_texcoord01 < 0.0f ? 0.0f : (v_texcoord01 > 1.0f ? 1.0f : v_texcoord01);
            float v_scaled = v_texcoord01 * k_255;
            v_scaled = v_scaled < 0.0f ? 0.0f : (v_scaled > k_254 ? k_254 : v_scaled);
            int v_texel = (int)floor(v_scaled + k_half);

            scratch[v].position.x = source->position.x + depth_offset.i;
            scratch[v].position.y = source->position.y + depth_offset.j;
            scratch[v].position.z = source->position.z + depth_offset.k;
            scratch[v].packed_texcoord = ((u_texel & 0xFF) << 16) | ((v_texel & 0xFF) << 8);
        }

        /* fill the decal datum */
        decal->position = collision->point;
        decal->creation_time = game_time_get();
        decal->sequence_index = (char)sequence_index;
        decal->sprite_index = 0;
        decal->bitmap_index = (char)decal_bitmap_index;
        {
            unsigned int *seed = get_global_local_random_seed_address();
            decal->lifetime = real_seed_random_range(seed, definition->lifetime_lower_bounds,
                                                     definition->lifetime_upper_bounds);
        }
        {
            unsigned int *seed = get_global_local_random_seed_address();
            decal->decay_time = real_seed_random_range(seed, definition->decay_time_lower_bounds,
                                                       definition->decay_time_upper_bounds);
        }
        decal->quad_count = quad_count;
        decal->definition_index = current_definition_index;

        float intensity_scalar;
        real_rgb_color color;
        {
            unsigned int *seed = get_global_local_random_seed_address();
            intensity_scalar = real_seed_random_range(seed, definition->intensity_lower_bounds,
                                                      definition->intensity_upper_bounds);
        }
        {
            unsigned int *seed = get_global_local_random_seed_address();
            float u = real_seed_random_range(seed, 0.0f, 1.0f);
            rgb_colors_interpolate(&color, (definition->flags >> 1) & 3,
                                   &definition->color_lower_bounds, &definition->color_upper_bounds, u);
        }
        decal->intensity = 0xFF;
        {
            unsigned int alpha = (uint8_t)(long long)(intensity_scalar * k_255);
            unsigned int red = (uint8_t)(long long)(color.red * k_255);
            unsigned int green = (uint8_t)(long long)(color.green * k_255);
            unsigned int blue = (uint8_t)(long long)(color.blue * k_255);
            decal->color = (alpha << 24) | (red << 16) | (green << 8) | blue;
        }

        /* expand each surface's triangle fan into the locked quad buffer (0x83742C58) */
        if (surface_count > 0)
        {
            int fan_base = 0;
            decal_render_vertex *write = locked;
            for (int s = 0; s < surface_count; s++)
            {
                int fan_count = geometry.decal_surface_vertex_counts[s];
                if (fan_count > 2)
                {
                    for (int i = 1; i + 1 < fan_count; i += 2)
                    {
                        int next = (i + 2 < fan_count) ? (fan_base + i + 2) : fan_base;
                        write[0] = scratch[fan_base];
                        write[1] = scratch[fan_base + i];
                        write[2] = scratch[fan_base + i + 1];
                        write[3] = scratch[fan_base];
                        write[4] = scratch[fan_base + i + 1];
                        write[5] = scratch[next];
                        write += 6;
                    }
                }
                fan_base += fan_count;
            }
        }

        rasterizer_decal_vertices_unlock();

        /* on success, advance to the next decal in the chain (0x83742E18) */
        chain_reuses_basis = (unsigned char)(definition->flags & (1u << _decal_definition_geometry_inherited_by_next_decal_in_chain_bit));
        current_definition_index = definition->next_decal_in_chain.index;
    }
}
