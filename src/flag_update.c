/* flag_update @0x8380EE60 — per-frame cloth relaxation step for a placed CTF/objective flag. Re-anchors the
 * simulation to its attachment markers (flag_update_attachment), then, if the simulation has started
 * (flag->__noop clear) and the flag has any width, walks every cloth vertex column by column: each vertex is
 * free-flight integrated one step (gravity/wind/collision, via point_physics_update) using a small random wind
 * gust scaled by the physics tag's friction fields, then its final position is either snapped exactly to its
 * attachment marker (first column, on a row that IS an attachment point) or pulled toward a weighted average
 * of up to three grid neighbors (previous column, next row, previous row) plus — on the first column only —
 * the row's interpolated attachment anchor; the previous-column neighbor (and the attachment anchor) carry
 * 4x the weight of the row neighbors. Velocity is then backed out from the position delta over `time`.
 * Column sweep direction alternates by flag->update_state so the solver doesn't bias toward one edge.
 *
 * DEVIATIONS (all disasm-resolved; this function drove the decompiler's own "may be wrong" warning and
 * every punned expression below needed a raw register trace, not just a read of the decompiled text):
 *  - `time` is `float` per the database's own registered prototype, not the decompiler's `double`.
 *  - THE HEADLINE BUG: the decompiler's `leaf_index` local (used as `v67[leaf_index]`, i.e. as an index into
 *    the attachment_points array) is NOT `attachment_location.leaf_index` — disasm (0x8380F050-0x8380F06C)
 *    shows it is loaded from a completely different stack array, indexed by the CURRENT ROW with a 2-byte
 *    stride (`lhzx r30, r10, r9` with r10 = row*2, r9 = &y_attachments[0]) — it is `y_attachments[row]`, the
 *    per-row attachment-index map that flag_update_attachment itself builds. The decompiler's naming came
 *    from the coincidental field-shape match, not the actual data flow; reading it as a scenario `location`
 *    would silently produce nonsense. Corrected name: `attachment_index_for_row`.
 *  - The three neighbor grid offsets (decompiler's punned `__int128`/SWORD/DWORD1 soup building v63 indices)
 *    are, once traced register-by-register (0x8380EF10-0x8380EF78 and 0x8380F1C0-0x8380F1D8): column offsets
 *    {-1, 0, 0} paired with row offsets {0, 1, -1} — i.e. (previous column, same row), (same column, next
 *    row), (same column, previous row). Column offset pairs with `cell_width_scale`, row offset with
 *    `cell_height_scale` in the rest-length computation (both confirmed via the same disasm).
 *  - `location v66[10]` / `real_point3d v67[5]` / `real_point3d v69[46]` etc: this file uses
 *    flag_update_attachment's own parameter names (attachment_location, attachment_points,
 *    attachment_force_points, attachment_y, y_attachments) directly instead of the decompiler's opaque
 *    locals. The decompiler's `location v66[10]` is really a single `location` (index 0 only) plus unrelated
 *    float scratch the compiler packed into the unused tail of that stack slot for the neighbor-projection /
 *    rest-length values (`*((float*)&v66[7].leaf_index + n)` etc.) — reproduced here as independent, clearly
 *    named local arrays (`neighbor_rest_length[3]`, `neighbor_projected[3]`) with identical computed values;
 *    the aliasing itself has no behavioral consequence once separated out.
 *  - `v22 = *(int*)TAG_INSTANCE(physics.index)` and the later `*(point_physics_definition**)(TAG_INSTANCE_base
 *    + v25)` are the same single tag dereference computed twice; collapsed into one `physics_definition`
 *    local.
 *  - The backward column sweep (flag->update_state != 0) processes rows height-1 down to 1 and never touches
 *    row 0 on that pass: disasm (0x8380EFFC-0x8380F050) shows the bound check for the NEXT row happens
 *    before every iteration including the first, and the backward-only check is `row > 0` (not `row >= 0`).
 *    Reproduced faithfully as a single ternary-stepped `for` loop rather than "fixed", since skipping row 0
 *    on alternating frames is what the shipped binary actually does. */

#include <stdint.h>
#include "headers/flag_datum.h"
#include "headers/flag_definition.h"
#include "headers/location.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/point_physics_definition.h"
#include "headers/global_tag_instances.h"

extern void flag_update_attachment(flag_datum *flag, flag_definition *flag_definition, location *attachment_location, real_point3d *attachment_points, real_point3d *attachment_force_points, int16_t *attachment_y, int16_t *y_attachments);
extern uint8_t scenario_location_underwater(const location *location, const real_point3d *position, int16_t *optional_weather_palette_index);
extern uint32_t *get_global_local_random_seed_address(void);
extern real_vector3d *seed_random_direction3d(uint32_t *seed, real_vector3d *direction);
extern unsigned int point_physics_update(unsigned int flags, const point_physics_definition *definition, location *location, int16_t force_weather_palette_index, real_point3d *position, real_vector3d *translational_velocity, const real_vector3d *translational_force, real_vector3d *collision_normal, int16_t *collision_material_type, float radius, float dt);
extern float __fsqrts(float x);
extern float __fabs(float x);

void flag_update(flag_datum *flag, flag_definition *flag_definition, float time)
{
    unsigned __int8 sweep_forward = flag->update_state == 0;

    location attachment_location;
    real_point3d attachment_points[5];
    real_point3d attachment_force_points[46];
    __int16 attachment_y[8];
    __int16 y_attachments[40];
    flag_update_attachment(flag, flag_definition, &attachment_location, attachment_points, attachment_force_points,
            attachment_y, y_attachments);

    __int16 weather_palette_index;
    unsigned __int8 underwater = scenario_location_underwater(&attachment_location,
            &attachment_points[flag_definition->attachment_points.count - 1], &weather_palette_index);

    if ( flag->__noop )
        return;

    float cell_width_scale = flag_definition->cell_width_scale;
    float cell_height_scale = flag_definition->cell_height_scale;

    /* (previous column, same row), (same column, next row), (same column, previous row) */
    static const int column_offset[3] = { -1, 0, 0 };
    static const int row_offset[3]    = { 0, 1, -1 };
    float neighbor_rest_length[3];
    for ( int i = 0; i < 3; ++i )
    {
        float scaled_column = (float)column_offset[i] * cell_width_scale;
        float scaled_row = (float)row_offset[i] * cell_height_scale;
        neighbor_rest_length[i] = __fsqrts(scaled_row * scaled_row + scaled_column * scaled_column);
    }

    if ( flag_definition->width <= 0 )
        return;

    const point_physics_definition *physics_definition =
            TAG_GET(const point_physics_definition, flag_definition->physics.index);

    for ( int column = 0; column < flag_definition->width; ++column )
    {
        /* forward sweep visits rows [0, height); backward visits (0, height-1] — row 0 is genuinely never
         * processed on a backward pass, confirmed by the bound check ordering in disasm (0x8380F048 tests
         * the NEXT row's validity, including the initial row, before every iteration). */
        int row_start = sweep_forward ? 0 : flag_definition->height - 1;
        int row_step = sweep_forward ? 1 : -1;
        for ( int row = row_start; sweep_forward ? (row < flag_definition->height) : (row > 0); row += row_step )
        {
            flag_vertex_datum *vertex = &flag->vertices[flag_definition->height * column + row];

            unsigned int physics_flags;
            float wind_scale;
            if ( underwater )
            {
                physics_flags = 3;
                wind_scale = physics_definition->water_friction * flag_definition->wind_noise * 0.00016f;
            }
            else
            {
                physics_flags = 1;
                wind_scale = physics_definition->air_friction * flag_definition->wind_noise * 0.00039999999f;
            }

            real_vector3d wind_force;
            seed_random_direction3d(get_global_local_random_seed_address(), &wind_force);
            wind_force.n[0] *= wind_scale;
            wind_force.n[1] *= wind_scale;
            wind_force.n[2] *= wind_scale;

            location physics_location = attachment_location;
            real_point3d integrated_position = vertex->p;
            __int16 collision_material_type;
            point_physics_update(physics_flags, physics_definition, &physics_location, weather_palette_index,
                    &integrated_position, &vertex->v, &wind_force, 0, &collision_material_type, 0.02f, time);

            real_point3d final_position;
            __int16 attachment_index_for_row = y_attachments[row];

            if ( column != 0 || attachment_index_for_row == -1 )
            {
                real_point3d neighbor_projected[3];
                int neighbor_count = 0;

                for ( int i = 0; i < 3; ++i )
                {
                    int neighbor_column = column + column_offset[i];
                    int neighbor_row = row + row_offset[i];
                    if ( neighbor_column < 0 || neighbor_column >= flag_definition->width || neighbor_row < 0
                            || neighbor_row >= flag_definition->height )
                        continue;

                    flag_vertex_datum *neighbor = &flag->vertices[flag_definition->height * neighbor_column
                            + neighbor_row];
                    float dx = integrated_position.n[0] - neighbor->p.n[0];
                    float dy = integrated_position.n[1] - neighbor->p.n[1];
                    float dz = integrated_position.n[2] - neighbor->p.n[2];
                    float distance = __fsqrts(dx * dx + dy * dy + dz * dz);
                    if ( __fabs(distance) >= 0.000099999997f )
                    {
                        float inverse_distance = 1.0f / distance;
                        dx *= inverse_distance;
                        dy *= inverse_distance;
                        dz *= inverse_distance;
                    }

                    neighbor_projected[neighbor_count].n[0] = neighbor->p.n[0] + dx * neighbor_rest_length[i];
                    neighbor_projected[neighbor_count].n[1] = neighbor->p.n[1] + dy * neighbor_rest_length[i];
                    neighbor_projected[neighbor_count].n[2] = neighbor->p.n[2] + dz * neighbor_rest_length[i];
                    ++neighbor_count;
                }

                float weight_sum = 0.0f;
                float weighted_x = 0.0f, weighted_y = 0.0f, weighted_z = 0.0f;
                for ( int i = 0; i < neighbor_count; ++i )
                {
                    /* the previous-column neighbor is always recorded first (column>0 guarantees it's in
                     * bounds), so index 0 here is unambiguously that neighbor */
                    float weight = (column == 0 || i != 0) ? 1.0f : 4.0f;
                    weight_sum += weight;
                    weighted_x += neighbor_projected[i].n[0] * weight;
                    weighted_y += neighbor_projected[i].n[1] * weight;
                    weighted_z += neighbor_projected[i].n[2] * weight;
                }

                if ( column == 0 )
                {
                    weight_sum += 4.0f;
                    weighted_x += attachment_force_points[row].n[0] * 4.0f;
                    weighted_y += attachment_force_points[row].n[1] * 4.0f;
                    weighted_z += attachment_force_points[row].n[2] * 4.0f;
                }

                final_position.n[0] = weighted_x / weight_sum;
                final_position.n[1] = weighted_y / weight_sum;
                final_position.n[2] = weighted_z / weight_sum;
            }
            else
            {
                final_position = attachment_points[attachment_index_for_row];
            }

            vertex->v.n[0] = (final_position.n[0] - vertex->p.n[0]) * (1.0f / time);
            vertex->v.n[1] = (final_position.n[1] - vertex->p.n[1]) * (1.0f / time);
            vertex->v.n[2] = (final_position.n[2] - vertex->p.n[2]) * (1.0f / time);
            vertex->p = final_position;
        }
    }
}
