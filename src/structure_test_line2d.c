/* structure_test_line2d @0x8381C6CC — tests whether the straight 2D segment p0->p1 (p0 known to lie inside
 * `p0_surface_index`) stays within the structure BSP's collision surface boundary, walking each surface's edge
 * loop and hopping into "passable" neighbor surfaces (same pathfinding-surface bit6/bit7 breakable-refinement
 * test as `find_turning_point`/`structure_test_ray2d`) whenever the segment crosses one. Returns 0 (no
 * collision) with `result` describing where p1 landed, or 1 with `result` describing the blocking edge/point.
 *
 * Clean 7-param signature (no float args, no GPR-shadow hazard) — verified directly against the decompile, no
 * scramble found. Recurses into itself once (guarded by `recursed`) as a self-correction: if the segment
 * appears to exit every tested edge's outer half-plane but none of them is the actual crossing edge, it retries
 * from the surface's boundary-vertex centroid back toward p0 to re-derive a fallback surface index.
 *
 * BINARY BUG (kept faithful; verified instruction-by-instruction 2026-08-10): the neighbor surface index is
 * passed to the passability test with NO sentinel test and NO bound test, so a boundary edge — whose absent
 * neighbor is the -1 sentinel — reaches pathfinding_surfaces[-1]. The index is `edge->surface_indices[!on_right_side]`
 * (lwzx r14,r6,r11 @0x8381C7E0, r6 = 16 + !on_right_side*4), reloaded into r11 at the crossing branch
 * (lwzx r11,r6,r11 @0x8381C920) and used three instructions later as the table subscript
 * (lbzx r9,r11,r16 @0x8381C92C, r16 = structure->pathfinding_surfaces.address from lwz r16,0x1E8(r3) @0x8381C6D4).
 * The only compare on that index anywhere ahead of the read is cmpw cr6,r14,r23 @0x8381C7E8 — against
 * p1_surface_index, which merely sets `reached_target`; it is neither a -1 test nor a range test. The sole
 * compare between the reload and the read is cmplwi cr6,r10,0 @0x8381C928 on ignore_broken_surfaces (r19),
 * not on r11. The block has exactly one entry (bgt cr6,loc_8381C920 @0x8381C880) and no branch target lies
 * between 0x8381C920 and 0x8381C97C, so no guard can have been dropped from an unseen predecessor. The
 * passability test is inlined — the function's only calls are breakable_surface_flags_get, itself and
 * collision_surface_project_point2d — so there is no callee-side guard either. If the byte read out of bounds
 * happens to have bit 6 set, mr r30,r11 @0x8381C98C adopts -1 as the current surface and the next iteration
 * indexes surfaces[-1] as well (add r4,r11,r28 @0x8381C764, stride 12).
 * This is an as-built asymmetry, NOT a branch the decompiler dropped: the sibling structure_test_ray2d
 * @0x8381C420 runs the byte-identical inlined passability idiom and DOES carry a -1 test
 * (cmpwi cr6,r11,-1 @0x8381C4FC and @0x8381C580) — but placed AFTER the same unguarded read
 * (lbzx r9,r11,r24 @0x8381C490 and @0x8381C514), so even the guarded sibling reads pathfinding_surfaces[-1]
 * and only then rejects the sentinel. structure_test_line2d carries no -1 test at all.
 * Sentinel spelling: collision_edge::surface_indices is int[2] (DB types_members) and every compare on these
 * indices is signed (cmpw @0x8381C7E8, cmpwi ...,-1 @0x8381CA6C), while the read is an indexed lbzx — base
 * plus a 32-bit-wrapped index. A signed int subscript reproduces the shipped base-1 address exactly; spelling
 * it unsigned would compute base + 0xFFFFFFFF on a 64-bit host and diverge from the binary. */

#include <stdint.h>
#include "headers/structure_bsp.h"
#include "headers/collision_bsp.h"
#include "headers/real_point2d.h"
#include "headers/real_point3d.h"
#include "headers/path_collision_result.h"
#include "headers/PATHFINDING_SURFACE_flags.h"
#include "headers/blam_data_globals.h"
#include "headers/bit_vector.h"

extern const uint8_t *breakable_surface_flags_get(void);
extern float __fsqrts(float x);
extern real_point3d *collision_surface_project_point2d(const collision_bsp *bsp, int surface_index, int16_t projection_axis, uint8_t projection_sign, const real_point2d *p2d, real_point3d *p3d);

static uint8_t structure_test_line2d_surface_passable(const unsigned char *pathfinding_surfaces,
        const collision_surface *surfaces, const unsigned char *breakable_flags, uint8_t ignore_broken_surfaces,
        int surface_index)
{
    unsigned char flags = pathfinding_surfaces[surface_index];
    uint8_t passable = (flags >> PATHFINDING_SURFACE_WALKABLE_BIT) & 1;
    if (!ignore_broken_surfaces && passable && (flags & (1u << PATHFINDING_SURFACE_BREAKABLE_BIT)))
    {
        unsigned char breakable_index = surfaces[surface_index].breakable_surface_index;
        passable = BIT_VECTOR_TEST_FLAG((const unsigned int *)breakable_flags, breakable_index);
    }
    return passable;
}

uint8_t structure_test_line2d(const structure_bsp *structure, uint8_t ignore_broken_surfaces,
        const real_point2d *p0, int p0_surface_index, const real_point2d *p1, int p1_surface_index,
        path_collision_result *result)
{
    const collision_bsp *bsp = (const collision_bsp *)structure->collision_bsp.address;
    const unsigned char *pathfinding_surfaces = (const unsigned char *)structure->pathfinding_surfaces.address;
    const unsigned char *breakable_flags = breakable_surface_flags_get();

    float dx = p1->n[0] - p0->n[0];
    float dy = p1->n[1] - p0->n[1];
    uint8_t recursed = 0;
    uint8_t reached_target = 0;
    int surface_index = p0_surface_index;

    for (;;)
    {
        const collision_surface *surfaces = (const collision_surface *)bsp->surfaces.address;
        const collision_edge *edges = (const collision_edge *)bsp->edges.address;
        const collision_vertex *vertices = (const collision_vertex *)bsp->vertices.address;
        int first_edge_index = surfaces[surface_index].first_edge_index;
        int edge_index = first_edge_index;
        int edge_count = 0;
        uint8_t crossed_any = 0;
        real_point3d centroid = *global_origin3d;

        reached_target = 0;

        do
        {
            const collision_edge *edge = &edges[edge_index];
            uint8_t on_right_side = (edge->surface_indices[1] == surface_index);
            int neighbor_surface_index = on_right_side ? edge->surface_indices[0] : edge->surface_indices[1];

            const real_point3d *vertex_a = &vertices[on_right_side ? edge->vertex_indices[1] : edge->vertex_indices[0]].point;
            const real_point3d *vertex_b = &vertices[on_right_side ? edge->vertex_indices[0] : edge->vertex_indices[1]].point;

            float a_to_p0_y = vertex_a->n[1] - p0->n[1];
            float a_to_p0_x = vertex_a->n[0] - p0->n[0];
            float edge_dy = vertex_a->n[1] - vertex_b->n[1];
            float edge_dx = vertex_a->n[0] - vertex_b->n[0];
            float b_to_p1_y = p1->n[1] - vertex_b->n[1];
            float b_to_p1_x = p1->n[0] - vertex_b->n[0];
            float b_to_p0_x = vertex_b->n[0] - p0->n[0];
            float b_to_p0_y = vertex_b->n[1] - p0->n[1];

            if (neighbor_surface_index == p1_surface_index)
                reached_target = 1;

            centroid.n[0] += vertex_b->n[0];
            centroid.n[1] += vertex_b->n[1];
            centroid.n[2] += vertex_b->n[2];
            edge_count++;

            if (b_to_p1_y * edge_dx - edge_dy * b_to_p1_x > 0.0f)
            {
                crossed_any = 1;
                if (b_to_p0_y * dx - b_to_p0_x * dy > 0.0f && a_to_p0_x * dy - a_to_p0_y * dx > 0.0f)
                {
                    if (structure_test_line2d_surface_passable(pathfinding_surfaces, surfaces, breakable_flags,
                            ignore_broken_surfaces, neighbor_surface_index))
                    {
                        surface_index = neighbor_surface_index;
                        goto continue_outer;
                    }

                    /* edge epsilon = 1/128, nudges the crossing point off the edge line */
                    float edge_length = __fsqrts(edge_dy * edge_dy + edge_dx * edge_dx);
                    float t = -(edge_length * 0.0078125f - (b_to_p0_y * edge_dx - edge_dy * b_to_p0_x))
                            / (edge_dx * dy - edge_dy * dx);
                    real_point2d hit_point;
                    hit_point.n[1] = dy * t + p0->n[1];
                    hit_point.n[0] = dx * t + p0->n[0];
                    collision_surface_project_point2d(bsp, surface_index, 2, 1u, &hit_point, &result->point);
                    result->t = t;
                    result->edge_index = edge_index;
                    result->collision = 1;
                    result->surface_index = surface_index;
                    return 1;
                }
            }

            edge_index = on_right_side ? edge->edge_indices[1] : edge->edge_indices[0];
        }
        while (edge_index != first_edge_index);

        if (crossed_any)
        {
            centroid.n[0] *= 1.0f / edge_count;
            centroid.n[1] *= 1.0f / edge_count;

            if (!recursed && pathfinding_surfaces[surface_index])
            {
                real_point2d centroid_2d = { { centroid.n[0], centroid.n[1] } };
                path_collision_result recheck;
                if (!structure_test_line2d(structure, ignore_broken_surfaces, &centroid_2d, surface_index, p0, -1, &recheck))
                {
                    surface_index = recheck.surface_index;
                    recursed = 1;
                    continue;
                }
            }
            /* mirrors the original's `goto LABEL_21`: the recursive fallback failed to clear the segment, so
             * this is unconditionally treated as blocked at p0, skipping the surface_index/reached_target check
             * below entirely. */
            goto blocked_at_start;
        }
        break;

continue_outer:
        ;
    }

    if (surface_index == p1_surface_index || reached_target || p1_surface_index == -1)
    {
        collision_surface_project_point2d(bsp, surface_index, 2, 1u, p1, &result->point);
        result->t = 1.0f;
        result->edge_index = -1;
        result->collision = 0;
        result->surface_index = surface_index;
        return 0;
    }

blocked_at_start:
    collision_surface_project_point2d(bsp, p0_surface_index, 2, 1u, p0, &result->point);
    result->t = 0.0f;
    result->surface_index = -1;
    result->edge_index = -1;
    result->collision = 1;
    return 1;
}
