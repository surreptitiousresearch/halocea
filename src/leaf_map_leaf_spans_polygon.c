/* leaf_map_leaf_spans_polygon @0x8381AFF8 — test whether any face edge of a leaf_map leaf crosses a given
 * clip `plane` at a point that projects inside a 2D query polygon. For each face of the leaf, each face
 * edge is reconstructed to 3D (dominant-axis unprojection off the face's own BSP plane, exactly as in
 * leaf_map_get_leaf_bounds), and if that edge straddles the clip plane (endpoints on opposite sides by
 * more than 0.03) the crossing point is computed, projected into the query's 2D space
 * (projection / projection_sign → global_projection3d_mappings), and tested against the query polygon
 * (vertices2d, vertex_count) with a 0.05 tolerance. Returns 1 on the first edge whose crossing lands
 * inside the polygon, else 0.
 *
 * Sibling of leaf_map_get_leaf_bounds.c (shares the 24-byte leaf stride, the face/plane resolution, and
 * the dominant-axis 2D→3D unprojection idiom). The clip plane's fields are the DB names n/d. */

#include <stdint.h>
#include "headers/leaf_map.h"
#include "headers/map_leaf.h"
#include "headers/map_leaf_face.h"
#include "headers/bsp3d.h"
#include "headers/real_plane3d.h"
#include "headers/real_point3d.h"
#include "headers/real_point2d.h"

extern const int16_t global_projection3d_mappings[1][6][2];
extern float __fabs(float x);
extern uint8_t convex_hull2d_test_point(int16_t count, const real_point2d *points, const real_point2d *point, float epsilon);

int leaf_map_leaf_spans_polygon(const leaf_map *leaf_map, int leaf_index, real_plane3d *plane, int16_t projection, uint8_t projection_sign, int16_t vertex_count, real_point2d *vertices2d)
{
    /* recovered: (char *)leaves.address + 16*n + 8*(n & 0x7FFFFFFF) -> map_leaf[] indexing (24-byte stride) */
    const map_leaf *leaf = &((const map_leaf *)leaf_map->leaves.address)[leaf_index];

    if ( leaf->faces.count <= 0 )
        return 0;

    const map_leaf_face *faces = (const map_leaf_face *)leaf->faces.address;
    const int *nodes = (const int *)leaf_map->bsp->nodes.address;
    const char *planes = (const char *)leaf_map->bsp->planes.address;

    for ( int face_index = 0; face_index < leaf->faces.count; face_index = (int16_t)(face_index + 1) )
    {
        const map_leaf_face *face = &faces[face_index];
        const real_plane3d *face_plane =
                (const real_plane3d *)&planes[16 * nodes[3 * face->node_index]];
        const float *face_normal = face_plane->n.n;

        float abs_x = __fabs(face_normal[0]);
        float abs_y = __fabs(face_normal[1]);
        float abs_z = __fabs(face_normal[2]);
        int axis;
        if ( abs_z < abs_y || abs_z < abs_x )
            axis = (abs_y >= abs_x) ? 1 : 0;
        else
            axis = 2;

        int vertex_count_face = face->vertices.count;
        if ( vertex_count_face <= 0 )
            continue;

        float normal_axis = face_normal[axis];
        int face_projection = 2 * axis + (normal_axis > 0.0f);
        int u_axis = global_projection3d_mappings[0][face_projection][0];
        int v_axis = global_projection3d_mappings[0][face_projection][1];

        const real_point2d *face_vertices = (const real_point2d *)face->vertices.address;

        for ( int vertex = 0; vertex < vertex_count_face; )
        {
            /* unproject the current edge endpoint to 3D */
            const real_point2d *current2d = &face_vertices[vertex];
            real_point3d current;
            current.n[u_axis] = current2d->n[0];
            current.n[v_axis] = current2d->n[1];
            current.n[axis] = (__fabs(normal_axis) >= 0.000099999997f)
                    ? (-((face_normal[v_axis] * current2d->n[1])
                            - -((face_normal[u_axis] * current2d->n[0])
                                    - face_normal[3])) / normal_axis)
                    : 0.0f;

            /* unproject the next endpoint (wrapping to vertex 0) */
            int next_vertex = (vertex_count_face == vertex + 1) ? 0 : vertex + 1;
            const real_point2d *next2d = &face_vertices[next_vertex];
            real_point3d next;
            next.n[u_axis] = next2d->n[0];
            next.n[v_axis] = next2d->n[1];
            next.n[axis] = (__fabs(normal_axis) >= 0.000099999997f)
                    ? (-((face_normal[v_axis] * next2d->n[1])
                            - -((face_normal[u_axis] * next2d->n[0])
                                    - face_normal[3])) / normal_axis)
                    : 0.0f;

            float current_distance = (((plane->n.n[1] * current.n[1])
                    + (plane->n.n[2] * current.n[2]))
                    + (plane->n.n[0] * current.n[0])) - plane->d;
            float next_distance = ((plane->n.n[2] * next.n[2])
                    + ((plane->n.n[0] * next.n[0])
                            + (plane->n.n[1] * next.n[1]))) - plane->d;

            if ( (current_distance < -0.029999999f && next_distance > 0.029999999f)
              || (current_distance > 0.029999999f && next_distance < -0.029999999f) )
            {
                int query_projection = 2 * projection + projection_sign;
                int query_u_axis = global_projection3d_mappings[0][query_projection][0];
                int query_v_axis = global_projection3d_mappings[0][query_projection][1];

                float t = ((((plane->n.n[1] * current.n[1])
                                + (plane->n.n[2] * current.n[2]))
                                + (plane->n.n[0] * current.n[0])) - plane->d)
                        / ((plane->n.n[0] * (next.n[0] - current.n[0]))
                                + ((plane->n.n[2] * (next.n[2] - current.n[2]))
                                        + (plane->n.n[1] * (next.n[1] - current.n[1]))));

                real_point3d crossing;
                crossing.n[1] = ((next.n[1] - current.n[1]) * -t) + current.n[1];
                crossing.n[0] = ((next.n[0] - current.n[0]) * -t) + current.n[0];
                crossing.n[2] = ((next.n[2] - current.n[2]) * -t) + current.n[2];

                real_point2d crossing2d;
                crossing2d.n[0] = crossing.n[query_u_axis];
                crossing2d.n[1] = crossing.n[query_v_axis];

                if ( convex_hull2d_test_point(vertex_count, vertices2d, &crossing2d, 0.050000001f) )
                    return 1;
            }

            vertex_count_face = face->vertices.count;
            vertex = (int16_t)(vertex + 1);
        }
    }

    return 0;
}
