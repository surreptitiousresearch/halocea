/* king_engine_post_rasterize @0x8382D4A0 — debug/visualization draw for the "king of the hill" hill boundary.
 * Renders the hill polygon (king_globals.hill_points[0..hill_point_count-1]) as a ring of upright wall quads,
 * 0.8 world units tall, textured so the material tiles once per unit length around the perimeter.
 *
 * Two passes over the hill polygon:
 *   1. Accumulate the total perimeter length. segment_count = round(perimeter) (floor(perimeter + 0.5)); the
 *      texture is scaled so u runs 0..segment_count around the ring (texels_per_unit = segment_count/perimeter).
 *   2. For each edge current->next, build a vertical quad (current base, current top, next top, next base),
 *      compute its outward face normal, assign texcoords (u from the running perimeter fraction, v = 1.0 at the
 *      base and 0.2 at the top), and submit it via render_dynamic_quad with the multiplayer shader.
 *
 * DEVIATION: the decompiler shows a `long double` parameter, but the entry (0x8382D4A0-0x8382D4E4) never reads
 * the incoming float register — the slot is only scratch spill for intermediate doubles. The parameter was
 * dropped (B8 wave): the game_engine `post_rasterize` field is `void (*)(void)` and every sibling engine's
 * post_rasterize is void(void), so keeping a dead float made this the only member with a different type.
 *
 * DEVIATION: the DB decompile computes the face normal via a cross product in which several operand differences
 * are value-numbered to provably-zero forms (e.g. `quad[0].position.n[1] - quad[0].position.n[1]`, and
 * `current_x - current_x`). These are reproduced verbatim to preserve bit-exact float rounding; they reduce
 * algebraically to the wall's outward normal (-0.8*(next.y-current.y), 0.8*(next.x-current.x), 0). The
 * `__PAIR64__` 8-byte store into quad[3].position is reproduced as two adjacent float assignments (identical
 * memory image on this big-endian target). */

#include "headers/king_globals.h"
#include "headers/game_globals_multiplayer_information.h"
#include "headers/game_globals_tag.h"
#include "headers/model_vertex_uncompressed.h"
#include "headers/render_lighting.h"
#include "headers/render_animation.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include <string.h>
#include "headers/blam_data_globals.h"

/* global_game_globals (game_globals_tag *) is declared by game_globals_tag.h */

#include "headers/render_lighting.h"
#include "headers/render_animation.h"
extern float __fsqrts(float x);
extern double floor(double x);
extern float __fabs(float x);
extern void render_dynamic_quad(model_vertex_uncompressed *verts, int shader_index, const render_lighting *lighting, const render_animation *animation, float u_scale, float v_scale);

void king_engine_post_rasterize(void)
{
    float accumulated_distance = 0.0f;
    int hill_point_count = king_globals.hill_point_count;
    float perimeter = 0.0f;
    int shader_index = ((game_globals_multiplayer_information *)global_game_globals->multiplayer_information.address)->hill_shader.index;

    /* pass 1: perimeter length */
    if ( hill_point_count > 0 )
    {
        int next_index = 1;
        int remaining = hill_point_count;
        real_point3d *current_point = king_globals.hill_points;
        do
        {
            real_point3d *next_point =
                    &king_globals.hill_points[hill_point_count == next_index ? 0 : next_index];
            ++next_index;
            float dy = (next_point->n[1] - current_point->n[1]);
            float dz = (next_point->n[2] - current_point->n[2]);
            float edge_length = __fsqrts(
                    (((next_point->n[0] - current_point->n[0])
                                    * (next_point->n[0] - current_point->n[0]))
                            + ((dz * dz) + (dy * dy))));
            ++current_point;
            perimeter = (edge_length + perimeter);
            --remaining;
        }
        while ( remaining );
    }

    double segment_count = floor((perimeter + (float)0.5));
    float inverse_segment_count = (float)(1.0 / segment_count);
    float texels_per_unit = ((float)1.0 / ((float)(1.0 / segment_count) * perimeter));
    float previous_u = 0.0f;

    /* pass 2: emit a wall quad per edge */
    if ( hill_point_count > 0 )
    {
        int next_index = 1;
        int remaining = hill_point_count;
        real_point3d *current_point = king_globals.hill_points;
        do
        {
            real_point3d *next_point =
                    &king_globals.hill_points[hill_point_count == next_index ? 0 : next_index];
            float dy = (next_point->n[1] - current_point->n[1]);
            float dz = (next_point->n[2] - current_point->n[2]);
            float edge_length = __fsqrts(
                    (((next_point->n[0] - current_point->n[0])
                                    * (next_point->n[0] - current_point->n[0]))
                            + ((dz * dz) + (dy * dy))));
            float edge_end_u =
                    ((edge_length + accumulated_distance) * texels_per_unit);
            accumulated_distance = (edge_length + accumulated_distance);

            model_vertex_uncompressed quad[5];
            memset(quad, 0, 0x110u);

            float current_z = current_point->n[2];
            float current_x = current_point->n[0];
            quad[0].position.n[1] = current_point->n[1];
            quad[1].position.n[1] = quad[0].position.n[1];
            quad[0].position.n[2] = current_z;
            quad[1].position.n[0] = current_x;
            quad[0].position.n[0] = current_x;
            quad[1].position.n[2] = (current_z + (float)0.80000001);

            float next_y = next_point->n[1];
            float next_z = next_point->n[2];
            quad[2].position.n[0] = next_point->n[0];
            quad[2].position.n[1] = next_y;
            quad[2].position.n[2] = (next_z + (float)0.80000001);
            quad[3].position.n[0] = quad[2].position.n[0];   /* __PAIR64__ store: next.x */
            quad[3].position.n[1] = next_y;                  /* __PAIR64__ store: next.y */
            quad[3].position.n[2] = next_z;

            /* outward face normal (see DEVIATION note): the zero-difference operands are preserved verbatim */
            real_vector3d face_normal;
            float zero_dx = (current_x - current_x);
            face_normal.n[2] = ((next_y - quad[0].position.n[1]) * zero_dx)
                    - ((quad[2].position.n[0] - current_x)
                            * (quad[0].position.n[1] - quad[0].position.n[1]));
            face_normal.n[1] =
                    ((quad[2].position.n[0] - current_x)
                            * (quad[1].position.n[2] - current_z))
                    - ((quad[2].position.n[2] - quad[1].position.n[2]) * zero_dx);
            face_normal.n[0] =
                    ((quad[2].position.n[2] - quad[1].position.n[2])
                            * (quad[0].position.n[1] - quad[0].position.n[1]))
                    - ((next_y - quad[0].position.n[1]) * (quad[1].position.n[2] - current_z));

            float normal_length = __fsqrts(
                    ((face_normal.n[0] * face_normal.n[0])
                            + ((face_normal.n[2] * face_normal.n[2])
                                    + (face_normal.n[1] * face_normal.n[1]))));
            if ( __fabs(normal_length) >= 0.000099999997f )
            {
                float inverse_length = ((float)1.0 / normal_length);
                face_normal.n[0] = inverse_length
                        * (((quad[2].position.n[2] - quad[1].position.n[2])
                                        * (quad[0].position.n[1] - quad[0].position.n[1]))
                                - ((next_y - quad[0].position.n[1])
                                        * ((current_z + (float)0.80000001) - current_z)));
                face_normal.n[1] = (((quad[2].position.n[0] - current_x)
                                        * ((current_z + (float)0.80000001) - current_z))
                                - ((quad[2].position.n[2] - quad[1].position.n[2]) * zero_dx))
                        * inverse_length;
                face_normal.n[2] = (((next_y - quad[0].position.n[1]) * zero_dx)
                                - ((quad[2].position.n[0] - current_x)
                                        * (quad[0].position.n[1] - quad[0].position.n[1])))
                        * inverse_length;
            }

            quad[0].texcoord.n[1] = 1.0f;
            quad[1].texcoord.n[1] = 0.2f;
            quad[2].texcoord.n[1] = 0.2f;
            quad[3].texcoord.n[1] = 1.0f;
            quad[0].texcoord.n[0] = (previous_u * inverse_segment_count);
            quad[3].normal = face_normal;
            quad[1].texcoord.n[0] = quad[0].texcoord.n[0];
            quad[2].texcoord.n[0] = (edge_end_u * inverse_segment_count);
            quad[3].texcoord.n[0] = quad[2].texcoord.n[0];
            quad[2].normal = face_normal;
            quad[1].normal = face_normal;
            previous_u = edge_end_u;
            quad[0].normal = face_normal;

            render_dynamic_quad(quad, shader_index, nullptr, nullptr,
                    ((float)1.0 / inverse_segment_count), 1.0f);

            --remaining;
            ++current_point;
            ++next_index;
        }
        while ( remaining );
    }
}
