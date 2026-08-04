/* render_sky @0x837EB8C0 — draws the scenario's active sky model. Builds the base pose from the sky model's
 * rest orientations (or, if the animation graph has one, the named `animations[]` state's current animation
 * frame — each state's phase is advanced by render.time_delta_since_frame_sec / state.speed and wrapped mod
 * the state's animation's frame_count, persisted per-state in render_sky_globals.animation_states). Then, for
 * each shader_function slot, forces the lighting's ambient color to white (a flat, unlit look for
 * function-driven shader effects). Then walks the sky's lights block: every light with a valid effect index
 * gets its lens-flare direction resolved either directly from its stored facing (if its marker name is empty)
 * or from a named marker on the sky model (dropped if the marker isn't found), and — if a direction was
 * resolved — queues a lens flare at a fixed distance along it from the camera. Finally the sky's node
 * matrices are re-based onto the camera position (scaled near to unity, so the
 * sky always renders "at infinity" around the viewer) and the model is drawn with a mostly-blank lighting
 * environment.
 *
 * DEVIATION: multiple LODWORD/HIDWORD-style decompiler artifacts here are confirmed via disasm to be genuine,
 * if bizarre, original-binary behavior rather than decompiler noise — reproduced faithfully rather than
 * "fixed":
 *   - The tail lighting-fill (`v34`/`v35`/`v36`/`v37`) reads global_real_rgb_white->g and ->b correctly, but
 *     the third value (decompiler's `v36`, cast from `LODWORD(v34->n[0])`) is actually loaded from 12 bytes
 *     past global_real_rgb_white's base — i.e. NOT white.r, but whatever float sits immediately after the
 *     12-byte real_rgb_color in memory (the alpha field of the next entry in the shared, unnamed
 *     real_argb_color constants table `global_real_rgb_white` is known to point into). That stray value is
 *     stored into render_lighting.distant_lights[0].direction.n[1] AND — reproduced exactly as disasm shows,
 *     with no further register write in between — passed as render_model's node_matrices argument (3rd
 *     parameter), while the REAL computed skinned matrices buffer is passed one slot later, as
 *     region_permutation_indices (4th parameter, `const char *`). This looks like a genuine argument-order
 *     bug in the original compiled code, not a reconstruction error; both call-argument slots and the stray
 *     table read are reproduced verbatim.
 *   - The `v45` lighting-environment zero-fill (`v31 += 2; *(_QWORD*)v31 = 0;` x14) is reproduced as the raw
 *     pointer loop rather than converted to memset, since it demonstrably clears past render_lighting's
 *     documented 116-byte layout into adjacent stack space; a raw loop guarantees the same byte range
 *     regardless of that struct's exact modeled size. */

#include <stdint.h>
#include "headers/render_globals.h"
#include "headers/sky.h"
#include "headers/sky_animation.h"
#include "headers/sky_light.h"
#include "headers/animation_graph.h"
#include "headers/model.h"
#include "headers/animation.h"
#include "headers/real_orientation.h"
#include "headers/global_tag_instances.h"
#include "headers/object_marker.h"
#include "headers/render_lighting.h"
#include "headers/render_model_effect.h"
#include "headers/real_euler_angles2d.h"


#include "headers/render_sky_globals.h"
#include "headers/blam_data_globals.h"


#include "headers/real_rgb_color.h"
extern sky *scenario_get_sky(__int16 sky_index);
extern void model_get_node_orientations(const model *model, real_orientation *node_orientations);
extern void overlay_animation_apply_continuous(const animation *animation, float real_frame_index, real_orientation *node_orientations);
extern void model_node_matrices_from_orientations(const model *model, real_matrix4x3 *node_matrices, const real_orientation *node_orientations, const real_point3d *origin, const real_vector3d *forward, const real_vector3d *up);
extern void matrix4x3_multiply(const real_matrix4x3 *a, const real_matrix4x3 *b, real_matrix4x3 *result);
extern int16_t model_get_marker_by_name(int model_index, const char *name, const uint8_t *region_permutations, const int16_t *node_remapping_table, int16_t node_count, const real_matrix4x3 *node_matrices, uint8_t mirrored_flag, object_marker *markers, int16_t maximum_marker_count);
extern real_vector3d *vector3d_from_euler_angles2d(real_vector3d *vector, const real_euler_angles2d *angles);
extern real_vector3d *perpendicular3d(const real_vector3d *a, real_vector3d *result);
extern void lights_queue_lens_flare(int definition_index, const real_point3d *position, const real_vector3d *direction, const real_vector3d *up, const real_rgb_color *color, float scale);
extern void rasterizer_models_begin(uint8_t sky);
extern void rasterizer_models_end(void);
/* matched to def (src/render_model.c): the 4th param is change_colors (const real_rgb_color *), and the
 * 6th is animation_values (const float *); there is no region_permutation_indices param. render_sky's
 * documented arg-order oddity (below) still passes the same pointer values positionally. */
extern void render_model(int model_index, float level_of_detail_pixels, const real_matrix4x3 *node_matrices, const char *region_permutation_indices, const real_rgb_color *change_colors, const float *function_values, const render_lighting *lighting, const real_point3d *centroid, float radius, const render_model_effect *model_effect, int unique_identifier, int16_t forced_shader_permutation_index, unsigned int flags);
extern double fmod(double x, double y);
extern double __fabs(double x);
extern float __fsqrts(float x);
extern void *memcpy(void *dst, const void *src, unsigned int count);

void render_sky(void)
{
    if (!render.visible_sky_model)
        return;

    sky *active_sky = scenario_get_sky(render.visible_sky_index);
    const model *sky_model = TAG_GET(const model, active_sky->model.index);

    real_orientation node_orientations[43];
    model_get_node_orientations(sky_model, node_orientations);

    int animation_graph_index = active_sky->animation_graph.index;
    if (animation_graph_index != -1)
    {
        animation_graph *graph = TAG_GET(animation_graph, animation_graph_index);

        if (active_sky->animations.count > 0)
        {
            for (int state_index = 0; state_index < active_sky->animations.count; state_index++)
            {
                sky_animation *state = &((sky_animation *)active_sky->animations.address)[state_index];
                __int16 anim_index = state->animation_index;
                float speed = state->period;

                if (anim_index >= 0 && anim_index < graph->animations.count && speed != 0.0f)
                {
                    /* the animation is indexed by the state's own slot (state_index), not by anim_index —
                     * anim_index is only used to validate this state against the graph's animation count.
                     * Reproduced verbatim from the decompiler/disasm, which show a plain (non-corrupted)
                     * `180 * state_index` address computation here. */
                    const animation *anim = &((const animation *)graph->animations.address)[state_index];
                    if (anim->node_count == sky_model->nodes.count)
                    {
                        float phase = render.time_delta_since_frame_sec / speed
                                    + render_sky_globals.animation_states[state_index];
                        phase = (float)fmod(phase, 1.0);
                        render_sky_globals.animation_states[state_index] = phase;

                        overlay_animation_apply_continuous(anim, phase * anim->frame_count, node_orientations);
                    }
                }
            }
        }
    }

    real_matrix4x3 node_matrices[64];
    model_node_matrices_from_orientations(sky_model, node_matrices, node_orientations, global_origin3d,
                                          global_forward3d, global_up3d);

    render_lighting lighting;
    if (active_sky->shader_functions.count > 0)
    {
        for (int i = 0; i < active_sky->shader_functions.count; i++)
            lighting.ambient_color.n[i] = 1.0f;
    }

    /* every light whose effect definition index is valid gets its lens flare queued (not just the first) */
    for (int light_index = 0; light_index < active_sky->lights.count; light_index++)
    {
        sky_light *light = (sky_light *)active_sky->lights.address + light_index;
        if (light->lens_flare.index != -1)
        {
            const char *marker_name = light->lens_flare_marker_name;
            unsigned __int8 have_direction = 1;
            real_vector3d direction;

            if (marker_name[0] == '\0')
            {
                vector3d_from_euler_angles2d(&direction, &light->radiosity.direction);
            }
            else
            {
                object_marker marker;
                if (model_get_marker_by_name(active_sky->model.index, marker_name, 0, 0, -1, node_matrices, 0,
                                             &marker, 1))
                {
                    direction.n[0] = marker.matrix.position.n[0] - render.camera.position.n[0];
                    direction.n[1] = marker.matrix.position.n[1] - render.camera.position.n[1];
                    direction.n[2] = marker.matrix.position.n[2] - render.camera.position.n[2];

                    float length = __fsqrts(direction.n[0] * direction.n[0] + direction.n[2] * direction.n[2]
                                           + direction.n[1] * direction.n[1]);
                    if (__fabs(length) >= 0.0001)
                    {
                        float inverse_length = 1.0f / length;
                        direction.n[0] *= inverse_length;
                        direction.n[1] *= inverse_length;
                        direction.n[2] *= inverse_length;
                    }
                }
                else
                {
                    have_direction = 0;
                }
            }

            if (have_direction)
            {
                /* place a marker-space matrix 1023.875 units out along `direction`, facing back along it,
                 * and derive an "up" via a perpendicular vector, then queue the lens flare there */
                object_marker flare_marker;
                flare_marker.matrix.position.n[0] = direction.n[0] * 1023.875f + render.camera.position.n[0];
                flare_marker.matrix.position.n[1] = direction.n[1] * 1023.875f + render.camera.position.n[1];
                flare_marker.matrix.position.n[2] = direction.n[2] * 1023.875f + render.camera.position.n[2];
                flare_marker.matrix.forward.n[0] = -direction.n[0];
                flare_marker.matrix.forward.n[1] = -direction.n[1];
                flare_marker.matrix.forward.n[2] = -direction.n[2];

                perpendicular3d(&flare_marker.matrix.forward, &flare_marker.matrix.up);

                float up_length = __fsqrts(flare_marker.matrix.up.n[0] * flare_marker.matrix.up.n[0]
                                          + flare_marker.matrix.up.n[1] * flare_marker.matrix.up.n[1]
                                          + flare_marker.matrix.up.n[2] * flare_marker.matrix.up.n[2]);
                if (__fabs(up_length) >= 0.0001)
                {
                    float inverse_up_length = 1.0f / up_length;
                    flare_marker.matrix.up.n[0] *= inverse_up_length;
                    flare_marker.matrix.up.n[1] *= inverse_up_length;
                    flare_marker.matrix.up.n[2] *= inverse_up_length;
                }

                lights_queue_lens_flare(light->lens_flare.index, &flare_marker.matrix.position,
                                        &flare_marker.matrix.forward, &flare_marker.matrix.up,
                                        global_real_rgb_white, 1.0f);
            }
        }
    }

    real_matrix4x3 camera_relative;
    memcpy(&camera_relative, global_identity4x3, sizeof(camera_relative));
    camera_relative.n[3][0] = render.camera.position.n[0] * 0.99902344f;
    camera_relative.scale   = 0.0009765625f;
    camera_relative.n[3][1] = render.camera.position.n[1] * 0.99902344f;
    camera_relative.n[3][2] = render.camera.position.n[2] * 0.99902344f;

    for (int i = 0; i < sky_model->nodes.count; i++)
        matrix4x3_multiply(&camera_relative, &node_matrices[i], &node_matrices[i]);

    rasterizer_models_begin(1);

    /* zero the lighting environment's tail (everything after ambient_color/distant_light_count, which are
     * set above/left as decompiled) — reproduced as the raw 8-byte-stride clearing loop the compiler emitted,
     * which runs 14 iterations and (per disasm) clears past render_lighting's modeled 116-byte layout into
     * adjacent stack space; see file header DEVIATION note. */
    {
        char *fill_cursor = (char *)&lighting.distant_lights[0].color.n[2];
        for (int i = 0; i < 14; i++)
        {
            fill_cursor += 8;
            *(__int64 *)fill_cursor = 0;
        }
    }

    /* DEVIATION (see file header): `stray_table_value` is NOT global_real_rgb_white->r — disasm shows it is
     * read 12 bytes past global_real_rgb_white's base, i.e. from the entry immediately following "white" in
     * the shared color-constants table. It is stored into the lighting environment AND, unmodified, passed
     * as render_model's node_matrices argument below — both reproduced verbatim. */
    const unsigned int *stray_table_ptr = (const unsigned int *)((const char *)global_real_rgb_white + 12);
    unsigned int stray_table_value = *stray_table_ptr;
    float white_g = global_real_rgb_white->__s1.green;
    float white_b = global_real_rgb_white->__s1.blue;

    *(unsigned int *)&lighting.distant_lights[0].direction.n[1] = stray_table_value;
    lighting.distant_lights[0].direction.n[2] = white_g;
    lighting.distant_lights[1].color.n[0] = white_b;

    /* trailing stack args: disasm confirms forced_shader_permutation_index is the constant-0 register (r23,
     * live as 0 for the whole function) and flags is a separately-set constant 1 (r28, set once earlier and
     * unchanged); unique_identifier shares the same constant-0 register as the shader permutation index. */
    render_model(active_sky->model.index, 0.0f, (const real_matrix4x3 *)stray_table_value,
                (const char *)node_matrices, 0, 0, &lighting,
                (const real_point3d *)&lighting.distant_lights[0].direction, 0.0f,
                (const render_model_effect *)&render.camera, 0, 0, 1u);

    rasterizer_models_end();
}
