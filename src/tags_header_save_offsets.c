/* tags_header_save_offsets @0x83753A60 — walk every loaded model tag ('mod2') and, for each geometry part of
 * each region/geometry block, snapshot its runtime vertex/index buffer offsets so they can be restored after a
 * cache reload.
 *
 * The saved slots are the tag blocks' address fields: uncompressed_vertices.address holds the
 * vertex_buffer.hardware_format snapshot, triangles.address the triangle_buffer.hardware_format one. */

#include <stdint.h>
#include "headers/tag_iterator.h"
#include "headers/global_tag_instances.h"
#include "headers/model.h"
#include "headers/model_geometry.h"
#include "headers/gbxmodel_geometry_part.h"

extern void tag_iterator_new(tag_iterator *iterator, uint32_t key_group_tag);
extern int tag_iterator_next(tag_iterator *iterator);

void tags_header_save_offsets(void)
{
    tag_iterator iterator;
    tag_iterator_new(&iterator, 0x6D6F6432u);   /* 'mod2' */

    int tag_index;
    for ( tag_index = tag_iterator_next(&iterator); tag_index != -1; tag_index = tag_iterator_next(&iterator) )
    {
        model *model_tag = TAG_GET(model, tag_index);
        if ( model_tag->geometries.count > 0 )
        {
            int geometry_index = 0;
            do
            {
                model_geometry *geometry =
                    &((model_geometry *)model_tag->geometries.address)[geometry_index];
                if ( geometry->parts.count > 0 )
                {
                    int part_index = 0;
                    do
                    {
                        gbxmodel_geometry_part *part =
                            &((gbxmodel_geometry_part *)geometry->parts.address)[part_index];
                        part_index = (short)(part_index + 1);
                        void *saved = part->triangle_buffer.hardware_format;
                        part->uncompressed_vertices.address = part->vertex_buffer.hardware_format;
                        part->triangles.address = saved;
                    }
                    while ( part_index < geometry->parts.count );
                }
                geometry_index = (short)(geometry_index + 1);
            }
            while ( geometry_index < model_tag->geometries.count );
        }
    }
}
