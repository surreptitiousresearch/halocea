/* render_debug_leaf_faces @0x8381B4E8 — walks the leaf's face list, and for each face with more than 2
 * vertices, sweeps a fan index from 2 to vertex_count. Genuinely a no-op in this build: the sweep computes
 * `v7` but never issues any draw call — confirmed via disasm_range(0x8381B4E8,0x8381B554), which matches the
 * decompiler exactly (same dead-code-loop pattern as model_build_tangent_matrices.c). The per-leaf element is
 * map_leaf (24 bytes, faces tag_block at +0); each face is map_leaf_face (16 bytes), and the +4 "vertex_count"
 * is that face's vertices tag_block .count. */

#include <stdint.h>
#include "headers/leaf_map.h"
#include "headers/map_leaf.h"
#include "headers/map_leaf_face.h"

void render_debug_leaf_faces(const leaf_map *map, int leaf_index)
{
    map_leaf *leaf = &((map_leaf *)map->leaves.address)[leaf_index];
    int face_count = leaf->faces.count;

    if ( face_count > 0 )
    {
        map_leaf_face *faces = (map_leaf_face *)leaf->faces.address;

        for ( int i = 0; i < face_count; ++i )
        {
            int vertex_count = faces[i].vertices.count;

            if ( vertex_count > 2 )
            {
                for ( int16_t fan_index = 2; fan_index < vertex_count; ++fan_index )
                    ;
            }
        }
    }
}
