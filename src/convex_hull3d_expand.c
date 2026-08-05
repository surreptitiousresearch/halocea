/* convex_hull3d_expand @0x837FEF00 — add one point to an incrementally-built convex hull (Quickhull-style).
 * If the point already lies inside (or within global_convex_hull3d_delta of) every extant surface, it can't
 * expand the hull and the call trivially succeeds. Otherwise: surfaces the point sees (within
 * global_convex_hull3d_epsilon) are marked for removal; edges bordering exactly one removed surface become
 * "horizon" edges (their removed-side surface_index is severed to -1, and the first one found is remembered
 * as the walk's starting handle); each vertex's edge fan is spliced to drop removed edges (a vertex left with
 * none is dropped too); then the horizon loop is walked once, building one new triangular surface and one new
 * edge connecting the new point to each horizon edge, until the walk returns to its start — at which point the
 * new vertex is finalized and the loop is closed. Returns 0 if the point index is out of range, an edge
 * references an out-of-range surface, or there isn't a free vertex/surface/edge slot to grow into.
 *
 * Sibling of convex_hull3d_begin (initial-tetrahedron seed) and convex_hull3d_test_point (containment test,
 * whose disasm-verified 9-param prototype and phantom-parameter pattern is the same one resolved here).
 *
 * DEVIATIONS (all disasm-resolved):
 *  - The decompiler fabricated 19 extra trailing int parameters (a10..a27) plus mistyped the return as
 *    plain `int`; the database's own registered prototype is the 9-param, `unsigned __int8`-returning form
 *    used here (same phantom-parameter pattern already documented in convex_hull3d_test_point.c).
 *  - Four `X + 1 - (X + (X == -1))` index expressions (X = an edge's surface_indices[0] or [1]) are algebraic
 *    noise for the plain `(X == -1) ? 0 : 1` they compute; simplified to two named slot values
 *    (`far_slot_by_surface0`, `far_slot_by_surface1`) computed once per horizon edge. For a genuine horizon
 *    edge exactly one of surface_indices[0]/[1] is -1, so the two slots are always each other's complement —
 *    kept as two separate values (rather than one negated) purely because the decompiler derives them from
 *    two independently-read fields, not because they can differ in practice. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/vertex3d.h"
#include "headers/edge3d.h"
#include "headers/surface3d.h"
#include "headers/blam_data_globals.h"

extern real_plane3d *plane3d_from_points(real_plane3d *plane, const real_point3d *point0,
        const real_point3d *point1, const real_point3d *point2);

uint8_t convex_hull3d_expand(int16_t point_count, const real_point3d *points, int16_t vertex_count, vertex3d *vertices, int16_t edge_count, edge3d *edges, int16_t surface_count, surface3d *surfaces, int16_t point_index)
{
    if ( point_index < 0 || point_index >= point_count )
        return 0;

    const real_point3d *new_point = &points[point_index];

    /* Trivial accept: new_point is already inside every extant surface (within delta) — nothing to expand. */
    uint8_t already_contained = 1;
    for ( int16_t i = 0; i < surface_count; ++i )
    {
        surface3d *surface = &surfaces[i];
        if ( surface->extant )
        {
            float signed_distance = new_point->n[0] * surface->plane.normal.n[0]
                    + new_point->n[1] * surface->plane.normal.n[1]
                    + new_point->n[2] * surface->plane.normal.n[2] - surface->plane.d;
            if ( signed_distance > global_convex_hull3d_delta )
            {
                already_contained = 0;
                break;
            }
        }
    }
    if ( already_contained )
        return 1;

    /* Mark which surfaces survive (new_point is safely behind them) vs. are removed (new_point sees them). */
    for ( int16_t i = 0; i < surface_count; ++i )
    {
        surface3d *surface = &surfaces[i];
        if ( surface->extant )
        {
            float signed_distance = new_point->n[0] * surface->plane.normal.n[0]
                    + new_point->n[1] * surface->plane.normal.n[1]
                    + new_point->n[2] * surface->plane.normal.n[2] - surface->plane.d;
            surface->extant = signed_distance <= -global_convex_hull3d_epsilon;
        }
    }

    /* Classify edges: keep if either bordering surface survived; if exactly one survived, this is a horizon
     * edge — sever the link to the removed surface and remember the first horizon edge found. */
    int16_t first_horizon_edge = -1;
    for ( int16_t i = 0; i < edge_count; ++i )
    {
        edge3d *edge = &edges[i];
        if ( !edge->extant )
            continue;

        int surface0 = edge->surface_indices[0];
        int surface1 = edge->surface_indices[1];
        if ( surface0 < 0 || surface0 >= surface_count || surface1 < 0 || surface1 >= surface_count )
            return 0;

        uint8_t keep = surfaces[surface0].extant || surfaces[surface1].extant;
        edge->extant = keep;
        if ( keep && surfaces[surface0].extant != surfaces[surface1].extant )
        {
            if ( surfaces[surface0].extant )
                edge->surface_indices[1] = -1;
            else
                edge->surface_indices[0] = -1;
            if ( first_horizon_edge == -1 )
                first_horizon_edge = i;
        }
    }

    /* Splice each vertex's edge fan (circular linked list) to drop removed edges; drop vertices left bare. */
    for ( int16_t v = 0; v < vertex_count; ++v )
    {
        vertex3d *vertex = &vertices[v];
        if ( !vertex->extant )
            continue;

        int16_t first_surviving_edge = -1;
        int16_t last_surviving_edge = -1;
        int16_t edge_index = vertex->edge_index;
        do
        {
            edge3d *edge = &edges[edge_index];
            if ( edge->extant )
            {
                if ( last_surviving_edge == -1 )
                    first_surviving_edge = edge_index;
                else
                    edges[last_surviving_edge].edge_indices[v == edges[last_surviving_edge].vertex_indices[0]] =
                            edge_index;
                last_surviving_edge = edge_index;
            }
            edge_index = edge->edge_indices[v == edge->vertex_indices[0]];
        } while ( edge_index != vertex->edge_index );

        if ( last_surviving_edge == -1 )
        {
            vertex->extant = 0;
        }
        else
        {
            vertex->edge_index = first_surviving_edge;
            edges[last_surviving_edge].edge_indices[v == edges[last_surviving_edge].vertex_indices[0]] =
                    first_surviving_edge;
        }
    }

    /* First free vertex slot for the new point (vertex_count if the array is already full). */
    int16_t new_vertex_index = 0;
    while ( new_vertex_index < vertex_count && vertices[new_vertex_index].extant )
        ++new_vertex_index;
    if ( new_vertex_index >= vertex_count )
        return 0;

    /* Walk the horizon loop, adding one triangular surface + one new edge per horizon edge, closing the fan
     * once the walk returns to its starting edge. */
    int16_t start_edge = first_horizon_edge;
    int16_t horizon_edge_index = first_horizon_edge;
    int16_t previous_new_edge = -1;
    int16_t first_new_edge = -1;

    for ( ; ; )
    {
        int16_t new_surface_index = 0;
        while ( new_surface_index < surface_count && surfaces[new_surface_index].extant )
            ++new_surface_index;
        if ( new_surface_index >= surface_count )
            break;

        int16_t new_edge_index = 0;
        while ( new_edge_index < edge_count && edges[new_edge_index].extant )
            ++new_edge_index;
        if ( new_edge_index >= edge_count )
            break;

        edge3d *horizon_edge = &edges[horizon_edge_index];
        uint8_t far_slot_by_surface0 = horizon_edge->surface_indices[0] == -1 ? 0 : 1;
        uint8_t far_slot_by_surface1 = horizon_edge->surface_indices[1] == -1 ? 0 : 1;
        int16_t far_vertex = horizon_edge->vertex_indices[far_slot_by_surface1];
        int16_t near_vertex = horizon_edge->vertex_indices[far_slot_by_surface0];

        surface3d *new_surface = &surfaces[new_surface_index];
        new_surface->extant = 1;
        plane3d_from_points(&new_surface->plane, new_point, &points[vertices[near_vertex].point_index],
                &points[vertices[far_vertex].point_index]);
        new_surface->edge_index = horizon_edge_index;

        edge3d *new_edge = &edges[new_edge_index];
        new_edge->extant = 1;
        new_edge->vertex_indices[0] = far_vertex;
        new_edge->vertex_indices[1] = new_vertex_index;
        new_edge->edge_indices[0] = previous_new_edge;
        new_edge->edge_indices[1] = horizon_edge->edge_indices[far_slot_by_surface0];
        new_edge->surface_indices[0] = new_surface_index;
        new_edge->surface_indices[1] = -1;

        horizon_edge->edge_indices[far_slot_by_surface0] = new_edge_index;
        horizon_edge->surface_indices[far_slot_by_surface0] = new_surface_index;

        if ( previous_new_edge == -1 )
            first_new_edge = new_edge_index;
        else
            edges[previous_new_edge].surface_indices[1] = new_surface_index;
        previous_new_edge = new_edge_index;

        horizon_edge_index = new_edge->edge_indices[1];
        if ( horizon_edge_index == start_edge )
        {
            edge3d *closing_edge = &edges[first_new_edge];
            vertex3d *new_vertex = &vertices[new_vertex_index];
            new_vertex->extant = 1;
            new_vertex->point_index = point_index;
            new_vertex->edge_index = first_new_edge;
            int closing_surface = closing_edge->surface_indices[0];
            closing_edge->edge_indices[0] = new_edge_index;
            new_edge->surface_indices[1] = closing_surface;
            return 1;
        }
    }

    return 0;
}
