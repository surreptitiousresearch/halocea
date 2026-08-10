/* convex_hull3d_begin @0x837FE958 — seed an incremental 3D convex-hull build with an initial tetrahedron chosen
 * from the input point cloud. It picks four affinely-independent extreme points and writes the tetrahedron's
 * topology into the caller's vertex / edge / surface arrays (which must hold at least 4 / 6 / 4 entries), leaving
 * the remaining entries marked non-extant for convex_hull3d_expand to fill. Returns 0 (degenerate input) if any
 * of the four extreme points cannot be found or is within global_convex_hull3d_delta of the previous ones.
 *
 * The four extreme points are: the lowest-X point; the point farthest from it; the point farthest from the line
 * through those two; and the point farthest from the plane through those three. If that last signed distance is
 * positive the second/third points are swapped so the tetrahedron faces are wound consistently.
 *
 * DEVIATION: the decompiler carries the line direction through a stack real_plane3d scratch (v68) and a punned
 * __int64 (v44); that store is dead — plane3d_from_points overwrites the slot before it is read as a plane — so
 * the line direction is kept in a plain real_vector3d local here. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_plane3d.h"
#include "headers/vertex3d.h"
#include "headers/edge3d.h"
#include "headers/surface3d.h"
#include "headers/blam_data_globals.h"

extern double __fabs(double x);
extern real_plane3d *plane3d_from_points(real_plane3d *plane, const real_point3d *point0, const real_point3d *point1, const real_point3d *point2);

uint8_t convex_hull3d_begin(int16_t point_count, const real_point3d *points, int16_t vertex_count, vertex3d *vertices, int16_t edge_count, edge3d *edges, int16_t surface_count, surface3d *surfaces)
{
    int16_t min_x_index = -1;
    int16_t farthest_index = -1;
    int16_t line_farthest_index = -1;
    int16_t plane_farthest_index = -1;

    if ( vertex_count < 4 )
        return 0;
    if ( edge_count < 6 )
        return 0;
    if ( surface_count < 4 )
        return 0;

    /* 1. the lowest-X point */
    float min_x = 3.4028235e38f;
    for ( int i = 0; i < point_count; i = (int16_t)(i + 1) )
    {
        if ( points[i].n[0] < min_x )
        {
            min_x = points[i].n[0];
            min_x_index = i;
        }
    }
    if ( min_x_index == -1 )
        return 0;

    /* 2. the point farthest from it */
    float max_distance_sq = 0.0f;
    {
        const real_point3d *base = &points[min_x_index];
        float base_x = base->n[0];
        float base_y = base->n[1];
        float base_z = base->n[2];
        for ( int i = 0; i < point_count; i = (int16_t)(i + 1) )
        {
            float dx = (base_x - points[i].n[0]);
            float dy = (base_y - points[i].n[1]);
            float dz = (base_z - points[i].n[2]);
            float distance_sq = ((dx * dx) + ((dz * dz) + (dy * dy)));
            if ( distance_sq > max_distance_sq )
            {
                farthest_index = i;
                max_distance_sq = distance_sq;
            }
        }
    }
    if ( farthest_index == -1 )
        return 0;
    if ( max_distance_sq < global_convex_hull3d_delta )
        return 0;

    /* 3. the point farthest from the line through those two */
    float max_line_distance_sq = 0.0f;
    {
        const real_point3d *line_base = &points[min_x_index];
        float base_x = line_base->n[0];
        float base_y = line_base->n[1];
        float base_z = line_base->n[2];
        float dir_x = (points[farthest_index].n[0] - base_x);
        float dir_y = (points[farthest_index].n[1] - base_y);
        float dir_z = (points[farthest_index].n[2] - base_z);
        for ( int i = 0; i < point_count; i = (int16_t)(i + 1) )
        {
            const real_point3d *p = &points[i];
            float t = ((((p->n[0] - base_x) * dir_x)
                            + (((p->n[2] - base_z) * dir_z)
                                    + ((p->n[1] - base_y) * dir_y)))
                    / ((dir_x * dir_x) + ((dir_z * dir_z) + (dir_y * dir_y))));
            float perp_y = ((p->n[1] - base_y) - (dir_y * t));
            float perp_z = ((p->n[2] - base_z) - (dir_z * t));
            float perp_x = ((p->n[0] - base_x) - (t * dir_x));
            float perp_distance_sq = ((perp_x * perp_x)
                    + ((perp_z * perp_z) + (perp_y * perp_y)));
            if ( perp_distance_sq > max_line_distance_sq )
            {
                max_line_distance_sq = perp_distance_sq;
                line_farthest_index = i;
            }
        }
    }
    if ( line_farthest_index == -1 || max_line_distance_sq < global_convex_hull3d_delta )
        return 0;

    /* 4. the point farthest from the plane through those three */
    real_plane3d base_plane;
    plane3d_from_points(&base_plane, &points[min_x_index], &points[farthest_index], &points[line_farthest_index]);
    float max_plane_distance = 0.0f;
    for ( int i = 0; i < point_count; i = (int16_t)(i + 1) )
    {
        const real_point3d *p = &points[i];
        float signed_distance = (((p->n[0] * base_plane.n.n[0])
                        + ((p->n[1] * base_plane.n.n[1])
                                + (p->n[2] * base_plane.n.n[2])))
                - base_plane.d);
        if ( __fabs(signed_distance) > __fabs(max_plane_distance) )
        {
            max_plane_distance = signed_distance;
            plane_farthest_index = i;
        }
    }
    if ( plane_farthest_index == -1 || __fabs(max_plane_distance) < global_convex_hull3d_delta )
        return 0;

    if ( max_plane_distance > 0.0f )
    {
        int16_t swap = farthest_index;
        farthest_index = line_farthest_index;
        line_farthest_index = swap;
    }

    /* tetrahedron vertices */
    vertices[1].point_index = farthest_index;
    vertices[2].point_index = line_farthest_index;
    vertices[0].point_index = min_x_index;
    vertices[3].edge_index = 3;
    vertices[0].extant = 1;
    vertices[0].edge_index = 0;
    vertices[1].extant = 1;
    vertices[1].edge_index = 0;
    vertices[2].extant = 1;
    vertices[2].edge_index = 1;
    vertices[3].extant = 1;
    vertices[3].point_index = plane_farthest_index;

    /* tetrahedron edges (six) with their vertex / adjacent-edge / bordering-surface links */
    edges[0].edge_indices[1] = 3;
    edges[2].edge_indices[1] = 5;
    edges[2].surface_indices[1] = 3;
    edges[3].vertex_indices[1] = 3;
    edges[3].surface_indices[1] = 3;
    edges[4].vertex_indices[0] = 3;
    edges[4].edge_indices[1] = 5;
    edges[5].vertex_indices[0] = 3;
    edges[5].edge_indices[1] = 3;
    edges[5].surface_indices[1] = 3;
    edges[0].extant = 1;
    edges[0].vertex_indices[0] = 0;
    edges[0].vertex_indices[1] = 1;
    edges[0].edge_indices[0] = 1;
    edges[0].surface_indices[0] = 0;
    edges[0].surface_indices[1] = 1;
    edges[1].extant = 1;
    edges[1].vertex_indices[0] = 1;
    edges[1].vertex_indices[1] = 2;
    edges[1].edge_indices[0] = 2;
    edges[1].edge_indices[1] = 4;
    edges[1].surface_indices[0] = 0;
    edges[1].surface_indices[1] = 2;
    edges[2].extant = 1;
    edges[2].vertex_indices[0] = 2;
    edges[2].vertex_indices[1] = 0;
    edges[2].edge_indices[0] = 0;
    edges[2].surface_indices[0] = 0;
    edges[3].extant = 1;
    edges[3].vertex_indices[0] = 0;
    edges[3].edge_indices[0] = 4;
    edges[3].edge_indices[1] = 2;
    edges[3].surface_indices[0] = 1;
    edges[4].extant = 1;
    edges[4].vertex_indices[1] = 1;
    edges[4].edge_indices[0] = 0;
    edges[4].surface_indices[0] = 1;
    edges[4].surface_indices[1] = 2;
    edges[5].extant = 1;
    edges[5].vertex_indices[1] = 2;
    edges[5].edge_indices[0] = 1;
    edges[5].surface_indices[0] = 2;

    /* tetrahedron faces (four), each an outward-facing plane */
    surfaces[0].extant = 1;
    plane3d_from_points(&surfaces[0].plane, &points[min_x_index], &points[farthest_index],
            &points[line_farthest_index]);
    surfaces[0].edge_index = 0;
    surfaces[1].extant = 1;
    plane3d_from_points(&surfaces[1].plane, &points[min_x_index], &points[plane_farthest_index],
            &points[farthest_index]);
    surfaces[1].edge_index = 0;
    surfaces[2].extant = 1;
    plane3d_from_points(&surfaces[2].plane, &points[farthest_index], &points[plane_farthest_index],
            &points[line_farthest_index]);
    surfaces[2].edge_index = 1;
    surfaces[3].extant = 1;
    plane3d_from_points(&surfaces[3].plane, &points[min_x_index], &points[line_farthest_index],
            &points[plane_farthest_index]);
    surfaces[3].edge_index = 2;

    /* mark the unused tail of each array non-extant */
    for ( int i = 4; i < vertex_count; i = (int16_t)(i + 1) )
        vertices[i].extant = 0;
    for ( int i = 6; i < edge_count; i = (int16_t)(i + 1) )
        edges[i].extant = 0;
    for ( int i = 4; i < surface_count; i = (int16_t)(i + 1) )
        surfaces[i].extant = 0;

    return 1;
}
