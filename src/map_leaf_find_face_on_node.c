/* map_leaf_find_face_on_node @0x8381AA18 — linear search of a leaf's faces block for the face lying on
 * the given BSP node, returning its index or -1. The third parameter is accepted but never read
 * (matches the DB's 3-arg signature; disasm never touches r5).
 *
 * DEVIATION: the DB prototype names the args (const leaf_map *leaf_map, const map_leaf *leaf,
 * int node_index), but disasm reads a tag_block (count@0, address@4) from arg0 and compares each
 * 16-byte entry's word at +0 against arg1 — exactly map_leaf.faces iterating map_leaf_face entries
 * (node_index@0, vertices tag_block@4; DB-verified layouts). The DB's names are shifted by one:
 * arg0 is the map_leaf and arg1 is the node_index being searched for. Zero callers in the DB to
 * cross-check; typed here per the layout evidence. */

#include <stdint.h>
#include "headers/map_leaf.h"
#include "headers/map_leaf_face.h"

int16_t map_leaf_find_face_on_node(const map_leaf *leaf, int node_index, int unused_node_index)
{
    int count = leaf->faces.count;

    if (count <= 0)
        return -1;

    const map_leaf_face *faces = (const map_leaf_face *)leaf->faces.address;

    for (__int16 i = 0; i < count; ++i)
    {
        if (faces[i].node_index == node_index)
            return i;
    }
    return -1;
}
