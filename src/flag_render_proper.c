/* flag_render_proper @0x8380E3E8 — render one placed flag's simulated cloth mesh: builds a dynamic
 * vertex/triangle buffer from the flag's simulated grid (per-vertex tangent from a cross product of the
 * neighboring column/row deltas, plus row/column UV fractions), tesselates each grid cell into 0-2
 * triangles per its stored tesselation level, then submits the result through the model rasterizer using
 * the flag's team-colored shader (red/blue, selected by the carrying object's owner team).
 *
 * DEVIATION: the decompiler renders the tesselation==2 case as `if (tesselation == 2) break;` out of the
 * per-row scanning loop. This looked like it could skip the remaining rows in the column (the reason a
 * previous pass deferred this function), but tracing the disasm end to end shows the "break" target is just
 * the tesselation==2 triangle-emit block, physically laid out right after the loop by the compiler; it falls
 * straight through to the same per-row continue point every other case reaches (an explicit `goto LABEL_39`
 * right after case 3/5's emit, and simple fallthrough for case 4/0). No rows are skipped.
 *
 * DEVIATION: the decompiler's `v65.colors = (const real_rgb_color *)LODWORD(v68.n[0]);` (overwriting the
 * animation struct's first field right after copying it wholesale) is a decompiler artifact — this function
 * is flagged "local variable allocation has failed" at its top. Disasm at 0x8380EAF0/0x8380EB18 shows a
 * single 8-byte `ld`/`std` copy of the caller's `render_animation` (colors+values, exactly 8 bytes) into the
 * parameter block with no further field write; the animation is passed through unmodified, matching the
 * already-committed sibling render_dynamic_quad.c's handling of the same struct.
 *
 * DEVIATION: `sort_filth` (the final rasterizer_model_transparent_geometry_submit argument) is an
 * uninitialized decompiler local (v67, never assigned) — the same pattern already documented in
 * render_dynamic_quad.c, where disasm showed the real argument is the same always-zero register used to
 * clear rasterizer_globals.current_lock_operation at cleanup. Passed as nullptr here for the same reason.
 *
 * Also fixes a swapped pair in the already-committed flag_dynamic_vertex.h: column_fraction/row_fraction
 * were named backwards (see that header's own deviation note).
 */

#include <stdint.h>
#include "headers/flag_datum.h"
#include "headers/flag_definition.h"
#include "headers/flag_dynamic_vertex.h"
#include "headers/unit_datum.h"
#include "headers/rasterizer_model_begin_parameters.h"
#include "headers/rasterizer_triangle.h"
#include "headers/shader.h"
#include "headers/render_lighting.h"
#include "headers/render_animation.h"
#include "headers/render_sort_filth.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_matrix4x3.h"
#include "headers/rasterizer_globals.h"
#include "headers/rasterizer_lock_operation.h"
#include "headers/rasterizer_vertex_type.h"
#include "headers/global_tag_instances.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/blam_data_globals.h"

struct triangle_buffer;
struct vertex_buffer;
struct transparent_geometry_group;


#include "headers/triangle_buffer.h"
#include "headers/vertex_buffer.h"
#include "headers/render_sort_filth.h"
extern void *memset(void *dst, int value, unsigned int size);
extern void *memcpy(void *dst, const void *src, unsigned int size);
extern double __fabs(double x);
extern float  __fsqrts(float x);
extern int rasterizer_dynamic_triangles_new(int count);
extern int rasterizer_dynamic_vertices_new(int16_t type, int count);
extern void * rasterizer_dynamic_vertices_lock(int dynamic_vertex_buffer_index);
extern rasterizer_triangle *rasterizer_dynamic_triangles_lock(int dynamic_triangle_buffer_index);
extern void rasterizer_dynamic_triangles_unlock(int dynamic_triangle_buffer_index);
extern void rasterizer_dynamic_vertices_unlock(int dynamic_vertex_buffer_index);
extern void rasterizer_dynamic_triangles_delete(int dynamic_triangle_buffer_index);
extern void rasterizer_dynamic_vertices_delete(int dynamic_vertex_buffer_index);
extern uint8_t shader_type_is_transparent(int16_t shader_type);
extern void rasterizer_profile_enable(uint8_t enable);
extern void rasterizer_models_begin(uint8_t sky);
extern void rasterizer_model_begin(const rasterizer_model_begin_parameters *parameters, uint8_t do_not_change_z_stencil_states);
extern void rasterizer_model_transparent_geometry_submit(const shader *shader, int16_t shader_permutation_index, const triangle_buffer *triangle_buffer, int dynamic_triangle_buffer_index, int triangle_count, const vertex_buffer *vertex_buffer, int dynamic_vertex_buffer_index, const real_point3d *centroid, render_sort_filth *sort_filth);
extern void rasterizer_model_draw(const shader *shader, int16_t shader_permutation_index, const triangle_buffer *triangle_buffer, int dynamic_triangle_buffer_index, int triangle_count, const vertex_buffer *vertex_buffer, int dynamic_vertex_buffer_index);
extern void rasterizer_model_end(void);
extern void rasterizer_models_end(void);

void flag_render_proper(flag_datum *flag, flag_definition *flag_definition, const render_lighting *lighting,
        const render_animation *animation)
{
    unit_datum *carrier = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, flag->object_index)->datum);
    int shader_index = carrier->object.owner_team_index ? flag_definition->shader_blue.index : flag_definition->shader_red.index;
    if ( shader_index == -1 )
        shader_index = flag_definition->shader_blue.index;

    rasterizer_globals.current_lock_operation = _rasterizer_lock_flag;

    int width = flag_definition->width;
    int height = flag_definition->height;
    int dynamic_triangle_buffer_index = rasterizer_dynamic_triangles_new((int16_t)(2 * (height - 1) * (width - 1)));
    int dynamic_vertex_buffer_index = rasterizer_dynamic_vertices_new(_rasterizer_vertex_type_model_uncompressed, width * height);

    if ( dynamic_triangle_buffer_index != -1 && dynamic_vertex_buffer_index != -1 )
    {
        flag_dynamic_vertex *dst_verts = rasterizer_dynamic_vertices_lock(dynamic_vertex_buffer_index);
        rasterizer_triangle *triangles = rasterizer_dynamic_triangles_lock(dynamic_triangle_buffer_index);

        float row_scale = 1.0f / (float)(height - 1);
        float column_scale = 1.0f / (float)(width - 1);
        int vertex_count = 0;

        for ( int column = 0; column < width; ++column )
        {
            float column_fraction = (float)column * column_scale;

            for ( int row = 0; row < height; ++row )
            {
                int sample_column = (column >= width - 1) ? column - 1 : column;
                int sample_row = (row < height - 1) ? row : row - 1;
                int sample_index = sample_column * height + sample_row;
                int next_column_index = (sample_column + 1) * height + sample_row;

                float delta_column_x = flag->vertices[next_column_index].p.n[0] - flag->vertices[sample_index].p.n[0];
                float delta_column_y = flag->vertices[next_column_index].p.n[1] - flag->vertices[sample_index].p.n[1];
                float delta_column_z = flag->vertices[next_column_index].p.n[2] - flag->vertices[sample_index].p.n[2];
                float delta_row_x = flag->vertices[sample_index + 1].p.n[0] - flag->vertices[sample_index].p.n[0];
                float delta_row_y = flag->vertices[sample_index + 1].p.n[1] - flag->vertices[sample_index].p.n[1];
                float delta_row_z = flag->vertices[sample_index + 1].p.n[2] - flag->vertices[sample_index].p.n[2];

                flag_dynamic_vertex *dst = &dst_verts[vertex_count];
                dst->tangent.n[0] = delta_row_z * delta_column_y - delta_row_y * delta_column_z;
                dst->tangent.n[1] = delta_column_z * delta_row_x - delta_row_z * delta_column_x;
                dst->tangent.n[2] = delta_row_y * delta_column_x - delta_column_y * delta_row_x;

                float tangent_length = __fsqrts(dst->tangent.n[0] * dst->tangent.n[0]
                    + (dst->tangent.n[1] * dst->tangent.n[1] + dst->tangent.n[2] * dst->tangent.n[2]));
                if ( __fabs(tangent_length) >= 0.000099999997f )
                {
                    float inverse_length = 1.0f / tangent_length;
                    dst->tangent.n[0] *= inverse_length;
                    dst->tangent.n[1] *= inverse_length;
                    dst->tangent.n[2] *= inverse_length;
                }

                dst->position = flag->vertices[column * height + row].p;
                dst->column_fraction = column_fraction;
                dst->row_fraction = (float)row * row_scale;
                dst->unused3C = 0.5f;
                dst->unused40 = 0.5f;

                ++vertex_count;
            }
        }

        /* tesselate each grid cell (height*column+row indexing) into 0-2 triangles per its stored level */
        int triangle_count = 0;
        for ( int column = 0; column < width - 1; ++column )
        {
            for ( int row = 0; row < height - 1; ++row )
            {
                int16_t tesselation = flag->cells[column * (height - 1) + row].tesselation;
                int top_left = height * column + row;
                int top_right = top_left + 1;
                int bottom_left = height * (column + 1) + row;
                int bottom_right = bottom_left + 1;

                if ( (unsigned int)tesselation > 5 || tesselation == 1 )
                    continue;   /* no geometry in this cell */

                if ( tesselation == 0 )
                {
                    triangles[triangle_count].vertex_indices[0] = top_left;
                    triangles[triangle_count].vertex_indices[1] = bottom_left;
                    triangles[triangle_count].vertex_indices[2] = top_right;
                    ++triangle_count;
                    triangles[triangle_count].vertex_indices[0] = top_right;
                    triangles[triangle_count].vertex_indices[1] = bottom_left;
                    triangles[triangle_count].vertex_indices[2] = bottom_right;
                    ++triangle_count;
                    continue;
                }

                switch ( tesselation )
                {
                case 2:
                    triangles[triangle_count].vertex_indices[0] = top_left;
                    triangles[triangle_count].vertex_indices[1] = top_right;
                    triangles[triangle_count].vertex_indices[2] = bottom_left;
                    break;
                case 3:
                    triangles[triangle_count].vertex_indices[0] = top_left;
                    triangles[triangle_count].vertex_indices[1] = top_right;
                    triangles[triangle_count].vertex_indices[2] = bottom_right;
                    break;
                case 4:
                    triangles[triangle_count].vertex_indices[0] = top_left;
                    triangles[triangle_count].vertex_indices[1] = bottom_right;
                    triangles[triangle_count].vertex_indices[2] = bottom_left;
                    break;
                default:   /* 5 */
                    triangles[triangle_count].vertex_indices[0] = top_right;
                    triangles[triangle_count].vertex_indices[1] = bottom_right;
                    triangles[triangle_count].vertex_indices[2] = bottom_left;
                    break;
                }
                ++triangle_count;
            }
        }

        rasterizer_dynamic_triangles_unlock(dynamic_triangle_buffer_index);
        rasterizer_dynamic_vertices_unlock(dynamic_vertex_buffer_index);

        const shader *flag_shader = TAG_GET(const shader, shader_index);

        /* recovered: the centroid averages the four cloth-grid corner vertices. The decompiler's raw
         * (float*)(flag_base + 24*height) and (flag_base + 24*width*height) reads, indexed [1..3], land
         * EXACTLY on vertices[height-1].p and vertices[width*height-1].p — offset 28 + 24*k resolves to
         * k = height-1 and k = width*height-1 respectively (flag_datum.vertices is at 0x1C, 24-byte stride),
         * so they are on-element, not off-boundary as an earlier note assumed. Corners: vertices[0],
         * [height-1], [height*(width-1)], [width*height-1]. Feeds transparent sort/cull heuristics only. */
        real_point3d near_edge_corner = flag->vertices[height - 1].p;
        real_point3d far_edge_corner = flag->vertices[width * height - 1].p;
        real_point3d far_corner = flag->vertices[height * (width - 1)].p;

        real_point3d centroid;
        centroid.n[0] = ((far_edge_corner.n[0] + near_edge_corner.n[0]) + far_corner.n[0] + flag->vertices[0].p.n[0]) * 0.25f;
        centroid.n[1] = ((far_edge_corner.n[1] + near_edge_corner.n[1]) + far_corner.n[1] + flag->vertices[0].p.n[1]) * 0.25f;
        centroid.n[2] = ((far_edge_corner.n[2] + near_edge_corner.n[2]) + far_corner.n[2] + flag->vertices[0].p.n[2]) * 0.25f;

        rasterizer_model_begin_parameters parameters;
        memset(&parameters, 0, sizeof(parameters));
        parameters.unique_id = 1;
        parameters.skinning.node_matrix_count = 1;
        parameters.skinning.node_matrices = (real_matrix4x3 *)global_identity4x3;
        memcpy(&parameters.lighting, lighting, sizeof(parameters.lighting));
        parameters.animation = *animation;
        parameters.base_map_scale.n[0] = 1.0f;
        parameters.base_map_scale.n[1] = 1.0f;
        parameters.centroid = centroid;

        rasterizer_profile_enable(0);
        rasterizer_models_begin(0);
        rasterizer_model_begin(&parameters, 0);
        if ( shader_type_is_transparent(flag_shader->base.type) )
            rasterizer_model_transparent_geometry_submit(flag_shader, 0, nullptr, dynamic_triangle_buffer_index,
                triangle_count, nullptr, dynamic_vertex_buffer_index, &centroid, nullptr);
        else
            rasterizer_model_draw(flag_shader, 0, nullptr, dynamic_triangle_buffer_index, triangle_count, nullptr,
                dynamic_vertex_buffer_index);
        rasterizer_model_end();
        rasterizer_models_end();
        rasterizer_profile_enable(1u);
    }

    rasterizer_dynamic_triangles_delete(dynamic_triangle_buffer_index);
    rasterizer_dynamic_vertices_delete(dynamic_vertex_buffer_index);
    rasterizer_globals.current_lock_operation = 0;
}
