/* hud_draw_multitexture_overlay @0x8379ED20 — composites a HUD element from up to three texture maps and
 * draws it as a rotated screen quad. It builds the four quad vertices (rotating bounds by `rotation` and
 * scaling/offsetting into screen space), fills a rasterizer_dynamic_screen_geometry_parameters block (per-map
 * bitmap, texture/map scale, offset anchor, wrap mode and map-to-map blend function), then walks the overlay's
 * scripted effector "functions" — each reads an input (0=aim pitch, 1/2=zero, 3=rounds loaded, 4=rounds
 * remaining, 5=weapon heat, 6=an authored constant, 7=zoom level) and drives an output (a map offset x/y, a map
 * fade, a map tint, or the master offset) by interpolating an output scalar + color across the input range.
 * Finally it submits the quad via rasterizer_psuedo_dynamic_screen_quad_draw.
 *
 * Reconstructed from disassembly (0x8379ED20-0x8379F534); the decompiler printed "local variable allocation
 * has failed". Deviations:
 *   - DB prototype is authoritative: 8 args, `color` is arg8 (r10). The decompiler invented a 9th arg `a9` and
 *     a phantom `color` (r9); the real per-vertex color is arg8. `theta` (the DB param name) is renamed here to
 *     `rotation` because it collides with the global `theta` (a persistent animation-phase accumulator this
 *     function increments, disasm 0x8379F1A8/F1B0). `rotation` is a float, not the decompiler's long double —
 *     sin/cos take it directly.
 *   - The overlay's per-map raw-offset reads are resolved to the DB struct fields (the decompiler expressed
 *     them through the u16* `&overlay->flags`, so its `+4*i`/`+8*(i+7)`/`+2*(i+23)` indices are u16/byte-scaled
 *     and land on map_scale[i]/map_blending_function[i]).
 *   - The 220-byte effector record is the DB type multitexture_overlay_hud_element_effector_definition:
 *     input_type=source@0x44, output_channel=destination_type@0x40, output_type=destination@0x42,
 *     input bounds=in_bounds@0x48/0x4C, output scalar bounds=out_bounds@0x50/0x54, output color bounds=
 *     tint_color_lower/upper_bounds@0x98/0xA4.
 *   - The many __int64 locals (v40/v42/v47/v55/v64/v65/v70) are decompiler register-pair shadows, not real
 *     64-bit values; unwound to the individual int/float/pointer they carry. */

#include <stdint.h>
#include "headers/multitexture_overlay_hud_element_definition.h"
#include "headers/multitexture_overlay_hud_element_effector_definition.h"
#include "headers/hud_multitexture_overlay_effector_source.h"
#include "headers/hud_multitexture_overlay_effector_destination.h"
#include "headers/hud_multitexture_overlay_effector_type.h"
#include "headers/rasterizer_dynamic_screen_geometry_parameters.h"
#include "headers/weapon_interface_state.h"
#include "headers/dynamic_screen_vertex.h"
#include "headers/real_rectangle2d.h"
#include "headers/point2d.h"
#include "headers/real_vector2d.h"
#include "headers/real_point2d.h"
#include "headers/real_rgb_color.h"
#include "headers/real_vector3d.h"
#include "headers/real_euler_angles2d.h"
#include "headers/player_datum.h"
#include "headers/bitmap_data.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"


extern double sin(double x);
extern double cos(double x);
extern int local_player_get_player_index(int16_t local_player_index);
extern int16_t local_player_count(void);
extern int16_t player_control_get_zoom_level(int16_t local_player_index);
extern void unit_get_aiming_vector(int unit_index, real_vector3d *aiming_vector);
extern real_euler_angles2d *euler_angles2d_from_vector3d(real_euler_angles2d *angles, const real_vector3d *vector);
extern bitmap_data *bitmap_group_get_bitmap_from_sequence(int bitmap_group_index, int16_t sequence_index, int16_t frame_index);
extern uint8_t hud_draw_multitexture_overlay_get_current_weapon_definition(const player_datum *player, weapon_interface_state *weapon_state);
extern void scalars_interpolate(float a, float b, float t, float *result);
extern real_rgb_color *rgb_colors_interpolate(real_rgb_color *rgb_result, unsigned int flags, const real_rgb_color *rgb_lower_bound, const real_rgb_color *rgb_upper_bound, float u);
extern void rasterizer_psuedo_dynamic_screen_quad_draw(rasterizer_dynamic_screen_geometry_parameters *parameters, dynamic_screen_vertex *vertices);

void hud_draw_multitexture_overlay(
        const multitexture_overlay_hud_element_definition *overlay,
        __int16 local_player_index,
        const point2d *point,
        const real_rectangle2d *clip,
        const real_rectangle2d *bounds,
        const real_vector2d *xy_scale,
        float rotation,   /* DB name: theta (renamed to avoid the global `theta` clash) */
        unsigned int color)
{
    float sin_rotation = (float)sin(rotation);
    float cos_rotation = (float)cos(rotation);

    /* the three map offsets, copied out for the geometry params to point at */
    float map_offset_scratch[6];
    map_offset_scratch[0] = overlay->map_offset[0].n[0];
    map_offset_scratch[1] = overlay->map_offset[0].n[1];
    map_offset_scratch[2] = overlay->map_offset[1].n[0];
    map_offset_scratch[3] = overlay->map_offset[1].n[1];
    map_offset_scratch[4] = overlay->map_offset[2].n[0];
    map_offset_scratch[5] = overlay->map_offset[2].n[1];

    real_rgb_color map_tint_scratch[3] = { { { 0.0f, 0.0f, 0.0f } }, { { 0.0f, 0.0f, 0.0f } }, { { 0.0f, 0.0f, 0.0f } } };
    float map_fade_scratch[3] = { 1.0f, 1.0f, 1.0f };
    real_vector2d offset_scratch = { { 0.0f, 0.0f } };

    weapon_interface_state weapon_state;
    int player_index = local_player_get_player_index(local_player_index);
    hud_draw_multitexture_overlay_get_current_weapon_definition(
        (const player_datum *)DATA_ARRAY_ELEMENT(player_data, player_datum, player_index), &weapon_state);

    /* build the four rotated screen-space quad vertices */
    dynamic_screen_vertex vertices[6];
    for ( int i = 0; i < 4; i = (__int16)(i + 1) )
    {
        int corner_x = (i + 1) & 2;
        float tex_u = corner_x ? clip->n[1] : clip->n[0];
        float tex_v = (i <= 1) ? clip->n[2] : clip->n[3];
        float bound_x = corner_x ? bounds->n[1] : bounds->n[0];
        float bound_y = (i <= 1) ? bounds->n[2] : bounds->n[3];

        vertices[i].position.n[2] = 0.0f;
        vertices[i].texcoord.n[1] = tex_v;
        vertices[i].texcoord.n[0] = tex_u;
        vertices[i].color = color;

        int px = (int)(((bound_x * cos_rotation) - (bound_y * sin_rotation)) * xy_scale->n[0]) + point->n[0];
        int py = (int)(((bound_y * cos_rotation) + (bound_x * sin_rotation)) * xy_scale->n[1]) + point->n[1];
        vertices[i].position.n[0] = (float)px;
        vertices[i].position.n[1] = (float)py;
    }

    /* bulk-initialise the geometry params to the 0x00000011,0x00000000 dword pattern the compiler emits (139 bytes) */
    rasterizer_dynamic_screen_geometry_parameters params;
    char *init_cursor = (char *)&params - 8;
    for ( int n = 17; n; --n )
    {
        init_cursor += 8;
        *(long long *)init_cursor = 0x1100000000LL;
    }
    ((int *)init_cursor)[2] = 0;   /* faithful tail dword of the 139-byte bulk init */

    params.map_texture_scale[0].n[1] = 1.0f;
    params.map_texture_scale[0].n[0] = 1.0f;
    params.meter_parameters = nullptr;
    params.map_scale[0].n[1] = 1.0f;
    params.map_scale[0].n[0] = 1.0f;
    params.point_sampled = local_player_count() == 1;   /* disasm: (_cntlzw(count-1) & 0x20) != 0, i.e. count==1 */
    params.map[0] = bitmap_group_get_bitmap_from_sequence(overlay->map[0].index, 0, 0);
    params.map[1] = bitmap_group_get_bitmap_from_sequence(overlay->map[1].index, 0, 0);
    params.map[2] = bitmap_group_get_bitmap_from_sequence(overlay->map[2].index, 0, 0);

    /* the two map-to-map blend-function slots, addressed as a small pointer table indexed by map */
    __int16 *map_blend_slot[2] = { &params.map0_to_1_blend_function, &params.map1_to_2_blend_function };

    for ( int map_index = 0; map_index < 3; map_index = (__int16)(map_index + 1) )
    {
        bitmap_data *bitmap = params.map[map_index];
        if ( bitmap )
        {
            float scale_x = (overlay->map_scale[map_index].n[0] == 0.0f) ? 1.0f : (1.0f / overlay->map_scale[map_index].n[0]);
            float scale_y = (overlay->map_scale[map_index].n[1] == 0.0f) ? 1.0f : (1.0f / overlay->map_scale[map_index].n[1]);

            if ( ((bitmap->width - 1) & bitmap->width) != 0 || ((bitmap->height - 1) & bitmap->height) != 0 )
            {
                /* non-power-of-two: normalise texcoords by the real dimensions */
                params.map_texture_scale[map_index].n[0] = 1.0f / (float)bitmap->width;
                params.map_texture_scale[map_index].n[1] = 1.0f / (float)bitmap->height;
            }
            else
            {
                params.map_texture_scale[map_index].n[0] = 1.0f;
                params.map_texture_scale[map_index].n[1] = 1.0f;
            }

            params.map_offset[map_index] = (real_point2d *)&map_offset_scratch[2 * map_index];
            params.map_scale[map_index].n[0] = scale_x;
            params.map_scale[map_index].n[1] = scale_y;
            params.map_wrapped[map_index] = overlay->map_clamp[map_index];
        }

        if ( map_index >= 2 )
        {
            params.framebuffer_blend_function = overlay->framebuffer_blend_function;
        }
        else
        {
            unsigned __int16 blend = overlay->map_blending_function[map_index];
            if ( blend <= 4 )
            {
                if ( blend == 1 )
                    *map_blend_slot[map_index] = 2;
                else if ( blend == 2 )
                    *map_blend_slot[map_index] = 1;
                else if ( blend == 3 )
                    *map_blend_slot[map_index] = 3;
                else if ( blend )   /* blend == 4 */
                    *map_blend_slot[map_index] = 4;
                else
                    *map_blend_slot[map_index] = 0;
            }
        }
    }

    /* apply each scripted effector function */
    if ( overlay->functions.count > 0 )
    {
        float input_value = *(float *)&map_blend_slot[0];   /* v74 seed: garbage float bit-copy, matches shipped code */
        for ( int f = 0; f < overlay->functions.count; f = (__int16)(f + 1) )
        {
            multitexture_overlay_hud_element_effector_definition *effector =
                &((multitexture_overlay_hud_element_effector_definition *)overlay->functions.address)[f];
            theta = theta + 0.050000001f;

            unsigned int input_type = (unsigned __int16)effector->source;
            if ( input_type >= NUMBER_OF_HUD_MULTITEXTURE_OVERLAY_EFFECTOR_SOURCES )
                goto apply;

            if ( input_type == _hud_multitexture_overlay_effector_source_player_pitch_tangent
              || input_type == _hud_multitexture_overlay_effector_source_player_yaw )
            {
                input_value = 0.0f;
            }
            else if ( input_type == _hud_multitexture_overlay_effector_source_weapon_ammo_loaded )
            {
                input_value = (float)weapon_state.magazines[0].rounds_loaded;
            }
            else if ( input_type == _hud_multitexture_overlay_effector_source_weapon_ammo_total )
            {
                input_value = (float)weapon_state.magazines[0].rounds_remaining;
            }
            else if ( input_type == _hud_multitexture_overlay_effector_source_weapon_heat )
            {
                input_value = weapon_state.heat;
                goto apply;
            }
            else if ( input_type == _hud_multitexture_overlay_effector_source_explicit )
            {
                input_value = effector->in_bounds[0];
                goto apply;
            }
            else if ( input_type == _hud_multitexture_overlay_effector_source_player_pitch )
            {
                /* aim pitch */
                int aim_player_index = local_player_get_player_index(local_player_index);
                int unit_index = aim_player_index;
                if ( aim_player_index != -1 )
                    unit_index = DATA_ARRAY_ELEMENT(player_data, player_datum, aim_player_index)->unit_index;
                real_vector3d aim_vector;
                unit_get_aiming_vector(unit_index, &aim_vector);
                real_euler_angles2d aim_angles;
                euler_angles2d_from_vector3d(&aim_angles, &aim_vector);
                input_value = aim_angles.n[1];
                goto apply;
            }
            else
            {
                /* _hud_multitexture_overlay_effector_source_zoom_level */
                input_value = (float)player_control_get_zoom_level(local_player_index);
            }

        apply:
            {
                float input_lower = effector->in_bounds[0];
                float input_upper = effector->in_bounds[1];
                float output_lower = effector->out_bounds[0];
                float output_upper = effector->out_bounds[1];

                float output_scalar;
                real_rgb_color output_color;
                if ( input_upper == input_lower || output_upper == output_lower )
                {
                    output_scalar = output_lower;
                    output_color = effector->tint_color_lower_bounds;
                }
                else
                {
                    float t = ((input_value - input_lower) / (input_upper - input_lower));
                    if ( t < 0.0f )
                        t = 0.0f;
                    else if ( t > 1.0f )
                        t = 1.0f;
                    scalars_interpolate(output_lower, output_upper, t, &output_scalar);
                    rgb_colors_interpolate(&output_color, 0,
                        &effector->tint_color_lower_bounds, &effector->tint_color_upper_bounds, t);
                }

                unsigned int output_type = (unsigned __int16)effector->destination;
                if ( output_type >= NUMBER_OF_HUD_MULTITEXTURE_OVERLAY_EFFECTOR_DESTINATIONS )
                    goto next;

                __int16 output_channel = effector->destination_type;
                if ( output_type == _hud_multitexture_overlay_effector_destination_primary_map )
                {
                    if ( output_channel )
                    {
                        if ( output_channel == _hud_multitexture_overlay_effector_type_horizontal_offset )
                            params.map_offset[0]->n[0] = output_scalar + params.map_offset[0]->n[0];
                        else if ( output_channel == _hud_multitexture_overlay_effector_type_vertical_offset )
                            params.map_offset[0]->n[1] = params.map_offset[0]->n[1] + output_scalar;
                        else if ( output_channel == _hud_multitexture_overlay_effector_type_alpha )
                        {
                            map_fade_scratch[0] = output_scalar;
                            params.map_fade[0] = &map_fade_scratch[0];
                        }
                    }
                    else
                    {
                        map_tint_scratch[0] = output_color;
                        params.map_tint[0] = &map_tint_scratch[0];
                    }
                }
                else if ( output_type == _hud_multitexture_overlay_effector_destination_secondary_map )
                {
                    if ( output_channel )
                    {
                        if ( output_channel == _hud_multitexture_overlay_effector_type_horizontal_offset )
                            params.map_offset[1]->n[0] = params.map_offset[1]->n[0] + output_scalar;
                        else if ( output_channel == _hud_multitexture_overlay_effector_type_vertical_offset )
                            params.map_offset[1]->n[1] = params.map_offset[1]->n[1] + output_scalar;
                        else if ( output_channel == _hud_multitexture_overlay_effector_type_alpha )
                        {
                            map_fade_scratch[1] = output_scalar;
                            params.map_fade[1] = &map_fade_scratch[1];
                        }
                    }
                    else
                    {
                        map_tint_scratch[1] = output_color;
                        params.map_tint[1] = &map_tint_scratch[1];
                    }
                }
                else if ( output_type == _hud_multitexture_overlay_effector_destination_tertiary_map )
                {
                    if ( output_channel )
                    {
                        if ( output_channel == _hud_multitexture_overlay_effector_type_horizontal_offset )
                            params.map_offset[2]->n[0] = output_scalar + params.map_offset[2]->n[0];
                        else if ( output_channel == _hud_multitexture_overlay_effector_type_vertical_offset )
                            params.map_offset[2]->n[1] = output_scalar + params.map_offset[2]->n[1];
                        else if ( output_channel == _hud_multitexture_overlay_effector_type_alpha )
                        {
                            map_fade_scratch[2] = output_scalar;
                            params.map_fade[2] = &map_fade_scratch[2];
                        }
                    }
                    else
                    {
                        map_tint_scratch[2] = output_color;
                        params.map_tint[2] = &map_tint_scratch[2];
                    }
                }
                else   /* _hud_multitexture_overlay_effector_destination_geometry_offset */
                {
                    offset_scratch.n[0] = (output_channel == _hud_multitexture_overlay_effector_type_horizontal_offset) ? output_scalar : 0.0f;
                    offset_scratch.n[1] = (output_channel == _hud_multitexture_overlay_effector_type_vertical_offset) ? output_scalar : 0.0f;
                    params.offset = &offset_scratch;
                }
            }
        next:;
        }
    }

    rasterizer_psuedo_dynamic_screen_quad_draw(&params, vertices);
}
