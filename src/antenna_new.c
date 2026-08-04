/* antenna_new @0x8380D340 — allocate and initialize a runtime antenna object from an antenna tag definition.
 * Grabs a new datum from antenna_data (700-byte stride), zeroes the antenna header, then walks the definition's
 * vertex list building a chain of node positions: node[i] gets the running accumulated position (the sum of the
 * per-vertex position deltas at definition-vertex +116/+120/+124), zero velocity, and — when the definition
 * references a bitmap group and the vertex's sequence resolves to a sprite — a texture-coordinate scale derived
 * from the sprite's UV span, the bitmap width, and the group's registration value. One extra sentinel node is
 * written past the last vertex holding the final accumulated position. Returns the antenna datum index (or -1 if
 * the definition index is -1 or the datum pool is full).
 *
 * The antenna definition and the bitmap-group tag are resolved to the DB antenna_definition /
 * bitmap_group / bitmap_group_sequence / bitmap_group_sprite structs (vertices tag_block @196,
 * texture tag ref @44, sequences @84, etc. — see the headers).
 *
 * DEVIATION: Hex-Rays typed the datum as float* and so rendered `*(int*)(antenna+12) = -1` (li r8,-1) as
 * `v7[3] = NAN`; it is an int -1 (a node/index sentinel). The denominator's two OVERLAPPED-punned terms resolve
 * (disasm 0x8380D4F0-0x8380D52C) to (short)bitmap->width and (short)*(short*)(group+80), the latter times 2.0. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/bitmap_data.h"
#include "headers/antenna_datum.h"
#include "headers/antenna_definition.h"
#include "headers/antenna_vertex.h"
#include "headers/bitmap_group.h"
#include "headers/bitmap_group_sequence.h"
#include "headers/bitmap_group_sprite.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"


extern int datum_new(data_array *data);
extern bitmap_data *bitmap_group_try_and_get_bitmap(int bitmap_group_index, int16_t bitmap_index);

int antenna_new(int antenna_definition_index)
{
    if ( antenna_definition_index == -1 )
        return -1;

    const antenna_definition *definition = TAG_GET(const antenna_definition, antenna_definition_index);
    int antenna_index = datum_new(antenna_data);
    if ( antenna_index == -1 )
        return -1;

    antenna_datum *antenna = DATA_ARRAY_ELEMENT(antenna_data, antenna_datum, antenna_index);
    unsigned int vertex_count = definition->vertices.count;
    const antenna_vertex *definition_vertices = (const antenna_vertex *)definition->vertices.address;

    antenna->initialized = 0;
    antenna->last_attachment_location.n[2] = 0.0f;
    antenna->object_index = -1;
    antenna->last_attachment_location.n[1] = 0.0f;
    antenna->last_attachment_location.n[0] = 0.0f;
    antenna->updates_since_last_render = 0;
    antenna->definition_index = antenna_definition_index;
    /* branchless-signed idiom (((x&0x80000000)==0)+(x>=2))&1 == signed (int)vertex_count < 2 */
    antenna->__noop = (unsigned __int8)((int)vertex_count < 2);

    float pos_x = antenna->last_attachment_location.n[0];   /* all 0.0 — running accumulated node position */
    float pos_y = antenna->last_attachment_location.n[1];
    float pos_z = antenna->last_attachment_location.n[2];

    int i = 0;
    if ( (int)vertex_count > 0 )
    {
        do
        {
            antenna_vertex_datum *node = &antenna->vertices[i];
            const antenna_vertex *def_vertex = &definition_vertices[i];

            node->p.n[0] = pos_x;
            node->p.n[1] = pos_y;
            node->p.n[2] = pos_z;
            node->v.n[0] = 0.0f;
            node->v.n[1] = 0.0f;
            node->v.n[2] = 0.0f;
            node->sprite_scale = 0.0f;
            node->sprite_index = 0;

            int bitmap_group_index = definition->texture.index;
            if ( bitmap_group_index != -1 )
            {
                __int16 sequence_index = def_vertex->sequence_index;
                const bitmap_group *bitmap_group_def = TAG_GET(const bitmap_group, bitmap_group_index);
                if ( sequence_index >= 0 && sequence_index < bitmap_group_def->sequences.count )
                {
                    const bitmap_group_sequence *sequence =
                            &((const bitmap_group_sequence *)bitmap_group_def->sequences.address)[sequence_index];
                    if ( sequence->sprites.count )
                    {
                        const bitmap_group_sprite *sprite = (const bitmap_group_sprite *)sequence->sprites.address;
                        bitmap_data *bitmap = bitmap_group_try_and_get_bitmap(bitmap_group_index,
                                (unsigned short)sprite->bitmap_index);
                        if ( bitmap )
                        {
                            float denom = (sprite->bounds.n[1] - sprite->bounds.n[0])
                                            * (float)bitmap->width
                                        - (float)bitmap_group_def->sprite_spacing * 2.0f
                                        - 1.0f;
                            node->sprite_scale = def_vertex->length_to_next / denom;
                        }
                    }
                }
            }

            i = (__int16)(i + 1);
            pos_x += def_vertex->vector_to_next.n[0];
            pos_y += def_vertex->vector_to_next.n[1];
            pos_z += def_vertex->vector_to_next.n[2];
        }
        while ( i < (int)vertex_count );
    }

    antenna_vertex_datum *sentinel = &antenna->vertices[(__int16)i];
    sentinel->p.n[0] = pos_x;
    sentinel->p.n[1] = pos_y;
    sentinel->p.n[2] = pos_z;
    sentinel->v.n[0] = 0.0f;
    sentinel->v.n[1] = 0.0f;
    sentinel->v.n[2] = 0.0f;
    return antenna_index;
}
