/* find_turning_point @0x8381EA20 — walk the structure BSP's collision edge graph starting at
 * `first_edge_index`, looking for the vertex where a pill of `radius` cast from `p` first stops being blocked
 * going around the corner in the requested `clockwise` direction. At each edge, the "near" vertex (chosen by
 * which side of the edge has a relevant breakable surface) determines a perpendicular offset test against `p`
 * that decides whether the corner is a valid turning point; if not, the code walks the edge chain around that
 * vertex (via forward_edge/reverse_edge) looking for an edge whose neighboring surface matches the desired
 * `clockwise` side, and continues the outer walk from there. Terminates successfully (writing the winning
 * vertex's 2D position to `*result`, returns 1) when the walk returns to the vertex it was already circling;
 * fails (returns 0) if the edge chain search or the outer walk cycles back to its start without finding one.
 *
 * DEVIATION — the DB's own decompile (verified via disasm_range 0x8381EA20-0x8381ED30) has its `clockwise`
 * and `ignore_broken_surfaces` parameters' runtime USES swapped throughout the body (every comparison the
 * decompiler prints against `ignore_broken_surfaces` actually reads r25, the register holding `clockwise`;
 * the one genuine `ignore_broken_surfaces` gate is instead printed as `(char)result`), the loop's initial
 * edge pointer is printed as `24 * clockwise + v17` when disasm shows it is actually `24 * first_edge_index +
 * v17` (i.e. `&edges[first_edge_index]`), and the final 2-DWORD write goes to a fabricated 8th parameter
 * (`a8`) rather than the real 7th parameter `result`. The DB's own 7-param `funcs.prototype` for this function
 * is correct and used verbatim; the body below is a from-scratch reconstruction using that prototype plus
 * direct register tracing, not a patched version of the decompiler's text. `structure->pathfinding_surfaces`
 * (offset 0x1E8, previously an opaque gap in structure_bsp.h) is newly resolved here: a tag_block whose
 * `.address` is a byte-per-collision-surface pathfinding flags array (PATHFINDING_SURFACE_flags:
 * bit6 = PATHFINDING_SURFACE_WALKABLE_BIT, bit7 = PATHFINDING_SURFACE_BREAKABLE_BIT). */

#include <stdint.h>
#include "headers/structure_bsp.h"
#include "headers/collision_bsp.h"
#include "headers/real_point2d.h"
#include "headers/PATHFINDING_SURFACE_flags.h"
#include "headers/bit_vector.h"

extern double __fabs(double x);
extern float __fsqrts(float x);
extern const uint8_t *breakable_surface_flags_get(void);

static uint8_t find_turning_point_surface_side(const unsigned char *pathfinding_surfaces,
        const collision_surface *surfaces, const unsigned char *breakable_flags, uint8_t ignore_broken_surfaces,
        int surface_index)
{
    unsigned char flags = pathfinding_surfaces[surface_index];
    uint8_t side = (flags >> PATHFINDING_SURFACE_WALKABLE_BIT) & 1;

    if (!ignore_broken_surfaces && side && (flags & (1u << PATHFINDING_SURFACE_BREAKABLE_BIT)))
    {
        unsigned char breakable_index = surfaces[surface_index].breakable_surface_index;
        side = BIT_VECTOR_TEST_FLAG((const unsigned int *)breakable_flags, breakable_index);
    }

    return side;
}

uint8_t find_turning_point(const structure_bsp *structure, const real_point2d *p, float radius,
        int first_edge_index, uint8_t clockwise, uint8_t ignore_broken_surfaces, real_point2d *result)
{
    const collision_bsp *bsp = (const collision_bsp *)structure->collision_bsp.address;
    const unsigned char *pathfinding_surfaces = (const unsigned char *)structure->pathfinding_surfaces.address;
    const unsigned char *breakable_flags = breakable_surface_flags_get();
    const collision_edge *edges = (const collision_edge *)bsp->edges.address;
    const collision_vertex *vertices = (const collision_vertex *)bsp->vertices.address;
    const collision_surface *surfaces = (const collision_surface *)bsp->surfaces.address;

    int starting_vertex = -1;
    const collision_edge *current_edge = &edges[first_edge_index];
    int loop_reference = -1;

    for (;;)
    {
        uint8_t side_flag = find_turning_point_surface_side(pathfinding_surfaces, surfaces, breakable_flags,
                ignore_broken_surfaces, current_edge->surface_indices[0]);

        int vertex_a_index = side_flag ? current_edge->vertex_indices[1] : current_edge->vertex_indices[0];
        int vertex_b_index = side_flag ? current_edge->vertex_indices[0] : current_edge->vertex_indices[1];
        const real_point3d *vertex_a = &vertices[vertex_a_index].point;
        const real_point3d *vertex_b = &vertices[vertex_b_index].point;

        float edge_dx = vertex_b->x - vertex_a->x;
        float edge_dy = vertex_b->y - vertex_a->y;
        float dir_x = -edge_dx;
        float dir_y = edge_dy;
        float length = __fsqrts(dir_x * dir_x + dir_y * dir_y);
        if (__fabs(length) >= 0.000099999997f)
        {
            float inv_length = 1.0f / length;
            dir_x *= inv_length;
            dir_y *= inv_length;
        }

        float point_pos_x = p->n[0] + dir_y * radius;
        float point_pos_y = p->n[1] + dir_x * radius;
        float point_neg_x = p->n[0] - dir_y * radius;
        float point_neg_y = p->n[1] - dir_x * radius;

        uint8_t is_valid = 0;

        uint8_t side_test = ((vertex_a->y - point_pos_y) * edge_dy + (vertex_a->x - point_pos_x) * edge_dx) < 0.0f;
        if (side_test == clockwise)
        {
            float cross_pos = (vertex_a->x - point_pos_x) * edge_dy - (vertex_a->y - point_pos_y) * edge_dx;
            if (cross_pos < 0.0f)
                is_valid = 1;
        }

        float cross_neg = (vertex_a->x - point_neg_x) * edge_dy - (vertex_a->y - point_neg_y) * edge_dx;
        if (cross_neg < 0.0f)
            is_valid = 1;

        if (starting_vertex == -1)
            is_valid = 1;

        uint8_t xor_flag = side_flag != is_valid;
        int next_vertex_index = (clockwise == xor_flag) ? current_edge->vertex_indices[1] : current_edge->vertex_indices[0];

        if (next_vertex_index == loop_reference)
        {
            result->n[0] = vertices[next_vertex_index].point.x;
            result->n[1] = vertices[next_vertex_index].point.y;
            return 1;
        }

        if (next_vertex_index == starting_vertex)
            return 0;

        if (starting_vertex == -1)
            starting_vertex = next_vertex_index;

        int chain_start_edge_index = first_edge_index;
        for (;;)
        {
            uint8_t matches_end = (next_vertex_index == current_edge->vertex_indices[1]);
            int candidate_surface_index = matches_end ? current_edge->surface_indices[1] : current_edge->surface_indices[0];

            uint8_t refined_side = find_turning_point_surface_side(pathfinding_surfaces, surfaces,
                    breakable_flags, ignore_broken_surfaces, candidate_surface_index);

            if (refined_side == clockwise)
                break;

            int next_edge_index = matches_end ? current_edge->edge_indices[0] : current_edge->edge_indices[1];
            if (next_edge_index == chain_start_edge_index)
                return 0;

            current_edge = &edges[next_edge_index];
        }

        loop_reference = next_vertex_index;
    }
}
