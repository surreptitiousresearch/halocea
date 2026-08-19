/* render_sky @0x837EB8C0 — draws the scenario's active sky model. Builds the base pose from the sky model's
 * rest orientations (or, if the animation graph has one, the named `animations[]` state's current animation
 * frame — each state's phase is advanced by render.time_delta_since_frame_sec / state.speed and wrapped mod
 * the state's animation's frame_count, persisted per-state in render_sky_globals.animation_states). Then, for
 * each shader_function slot, drives that shader function value to 1.0 (a flat, fully-on look for
 * function-driven shader effects). Then walks the sky's lights block: every light with a valid effect index
 * gets its lens-flare direction resolved either directly from its stored facing (if its marker name is empty)
 * or from a named marker on the sky model (dropped if the marker isn't found), and — if a direction was
 * resolved — queues a lens flare at a fixed distance along it from the camera. Finally the sky's node
 * matrices are re-based onto the camera position (scaled near to unity, so the
 * sky always renders "at infinity" around the viewer) and the model is drawn with a cleared lighting
 * environment whose ambient colour is white.
 *
 * DEVIATION (ABI, corrected 2026-08-12): render_model's second parameter `level_of_detail_pixels` is a float,
 * so it is passed in f1 AND consumes r4's GPR slot; every pointer argument therefore sits one register later
 * than the decompiler shows, and r4 is never read by the callee (see the register roles enumerated in
 * src/render_model.c). Two earlier readings of this function followed the decompiler's shifted map and are
 * corrected here:
 *   - the buffer at r1+0x150 that the shader-function loop fills with 1.0 is the `function_values` argument
 *     (r8), not the lighting environment's ambient colour;
 *   - the 116-byte object at r1+0x170 is the `lighting` argument (r9). The 14 x `stdu` + trailing `stw`
 *     zero-fill covers exactly its 116 bytes, so it is a whole-object clear rather than one that runs off the
 *     end of the struct, and the three words taken from global_real_rgb_white land on its own ambient_color
 *     rather than on distant_lights[] fields. There is consequently no "white.red bit pattern in the
 *     node_matrices slot" oddity to reproduce: that was the float shadow register.
 * The global_real_rgb_white loads themselves are at displacements 0/+4/+8 off the loaded pointer — IDA renders
 * the first symbolically as `(private_real_argb_colors+4 - 0x82113F0C)(r11)`, whose value is displacement
 * ZERO, and the `+0xC` token on the neighbouring load is relative to the TABLE base, not to r11. No load in
 * this function reaches outside private_real_argb_colors[0]. */

#include <stdint.h>
#include <string.h>
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
extern sky *scenario_get_sky(int16_t sky_index);
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
                int16_t anim_index = state->animation_index;
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

    /* DEVIATION: this loop fills the shader FUNCTION VALUES buffer — `addi r9, r1, 0x1760+var_1610` (r1+0x150)
     * with `stfsx f29, r10, r9` at r10 = 4*i, @0x837EBA68-0x837EBA74 — which is the r8 argument of the
     * render_model call below, not the lighting environment. The element count is the 4 of
     * default_function_values[] (what render_model substitutes when this argument is null) and of
     * _object_datum.outgoing_function_values[] (what every other render_model caller passes). */
    float function_values[4];
    if (active_sky->shader_functions.count > 0)
    {
        for (int i = 0; i < active_sky->shader_functions.count; i++)
            function_values[i] = 1.0f;
    }

    /* every light whose effect definition index is valid gets its lens flare queued (not just the first) */
    for (int light_index = 0; light_index < active_sky->lights.count; light_index++)
    {
        sky_light *light = (sky_light *)active_sky->lights.address + light_index;
        if (light->lens_flare.index != -1)
        {
            const char *marker_name = light->lens_flare_marker_name;
            uint8_t have_direction = 1;
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

    /* DEVIATION: the compiler spelled this clear as 14 x `stdu r9, 8(r10)` from r1+0x170 plus a trailing
     * `stw r9, 8(r10)` (@0x837EBD1C-0x837EBD3C) — 112 + 4 = 116 bytes = sizeof(render_lighting), starting
     * exactly at the r9 argument of the render_model call below. It is a whole-object clear. */
    render_lighting lighting;
    memset(&lighting, 0, sizeof(lighting));

    /* DEVIATION: the three words read from global_real_rgb_white at displacements 0/+4/+8 are stored to
     * r1+0x170/0x174/0x178 (@0x837EBD74-0x837EBD7C) — the lighting environment's own ambient_color, i.e. a
     * plain real_rgb_color copy, not the distant_lights[] fields an earlier (shifted) reading produced. */
    lighting.ambient_color = *global_real_rgb_white;

    /* DEVIATION (see file header): argument registers at `bl render_model` @0x837EBD88 —
     * r3 = model index, f1 = 0.0 level_of_detail_pixels (r4 is its shadow and is never read),
     * r5 = node_matrices (`addi r5, r1, 0x1760+var_D70` @0x837EBD60), r6 = 0 region_permutation_indices,
     * r7 = 0 change_colors, r8 = function_values (r1+0x150), r9 = &lighting (r1+0x170),
     * r10 = &render.camera (`addi r10, r24, 0x14` @0x837EBD40, r24 = &render), f2 = 0.0 radius, then the
     * stack arguments model_effect = 0 @0x5C, unique_identifier = 0 @0x64,
     * forced_shader_permutation_index = 0 @0x6E and flags = 1 @0x74. */
    render_model(active_sky->model.index, 0.0f, node_matrices, 0, 0, function_values, &lighting,
                 &render.camera.position, 0.0f, 0, 0, 0, 1u);

    rasterizer_models_end();
}
