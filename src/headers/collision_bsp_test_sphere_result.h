/* collision_bsp_test_sphere_result — surfaces/edges/vertices/leaves of a collision BSP overlapping a query
 * sphere (collision_bsp_test_sphere). Layout verbatim from the database. */
#pragma once

typedef struct collision_bsp_test_sphere_result
{
    int surface_count;        /* 0x0000 */
    int surface_indices[256]; /* 0x0004 */
    int edge_count;           /* 0x0404 */
    int edge_indices[256];    /* 0x0408 */
    int vertex_count;         /* 0x0808 */
    int vertex_indices[256];  /* 0x080C */
    int leaf_count;           /* 0x0C0C */
    int leaf_indices[256];    /* 0x0C10 */
} collision_bsp_test_sphere_result;
