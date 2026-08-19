/* tags_header_register_vertex_and_index_buffers @0x83754438 — create HCEX static vertex and index buffers
 * sized for the cache's combined vertex/index buffer region, stream the cache's vertex and index data into
 * them (blocking), then patch every model ('mod2') geometry part to reference the new buffers and their base
 * offsets. Finally unlock both buffers.
 *
 * Layout note: index_buffers here holds the *vertex* data size and vertex_index_buffer_size the combined size,
 * so the index region size is (combined - vertex_size). (Matches the engine's field reuse.)
 *
 * Deviation: the model/geometry/part structures are accessed by raw byte offset (not typed in the database);
 * reproduced verbatim. Field map (per part dword index):
 *   part[9]  = source vertex offset   -> part[23] = part[9]  + vbuf_base
 *   part[15] = source index offset    -> part[19] = part[15] + ibuf_base
 *   part[25] = vertex buffer handle (hcex_models_vbuf), part[20] = index buffer handle (hcex_models_ibuf) */

#include <stdint.h>
#include "headers/cache_file_tags_header.h"
#include "headers/cache_file_read_request_params.h"
#include "headers/global_tag_instances.h"
#include "headers/model.h"
#include "headers/model_geometry.h"
#include "headers/gbxmodel_geometry_part.h"
#include "headers/tag_iterator.h"
#include "headers/blam_data_globals.h"

/* DEVIATION: dropped <windows.h> (only Sleep was used, and the header collides with the
 * repo's own Win32-boundary headers); declare Sleep as an extern like sibling cache files. */
extern void Sleep(unsigned int milliseconds);

extern void *hcex_stat_vbuf_create(int size);
extern void *hcex_stat_ibuf_create(int size);
extern int   hcex_stat_vbuf_lock(void *buffer, int offset, int size, void **out_data);
extern void  hcex_stat_vbuf_unlock(void *buffer);
extern int16_t cache_file_read(int tag_index, uint32_t offset, int size, void *buffer, const cache_file_read_request_params *params, uint8_t blocking, uint8_t data_file);
extern void tag_iterator_new(tag_iterator *iterator, uint32_t key_group_tag);
extern int tag_iterator_next(tag_iterator *iterator);

void tags_header_register_vertex_and_index_buffers(cache_file_tags_header *tags_header)
{
    int   vertex_size = (int)tags_header->index_buffers;
    int   index_size  = tags_header->vertex_index_buffer_size - vertex_size;
    void *vertex_data = 0;
    void *index_data  = 0;

    hcex_models_vbuf = hcex_stat_vbuf_create(vertex_size);
    int vbuf_base = hcex_stat_vbuf_lock(hcex_models_vbuf, 0, vertex_size, &vertex_data);
    hcex_models_ibuf = hcex_stat_ibuf_create(index_size);
    int ibuf_base = hcex_stat_vbuf_lock(hcex_models_ibuf, 0, index_size, &index_data);

    volatile unsigned char finished = 0;
    cache_file_read_request_params params;
    params.finished_func = 0;
    params.finished_flag = &finished;
    params.userdata = 0;

    cache_file_read(-1, (unsigned int)tags_header->vertex_buffers, vertex_size, vertex_data, &params, 1u, 0);
    while ( !finished )
        Sleep(0);

    cache_file_read(-1, (unsigned int)tags_header->vertex_buffers + (unsigned int)vertex_size, index_size,
                    index_data, &params, 1u, 0);
    while ( !finished )
        Sleep(0);

    tag_iterator iterator;
    tag_iterator_new(&iterator, 0x6D6F6432u);   /* 'mod2' */
    for ( int tag_index = tag_iterator_next(&iterator); tag_index != -1; tag_index = tag_iterator_next(&iterator) )
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
                        int source_vertex = (int)part->uncompressed_vertices.address;
                        part->vertex_buffer.hardware_format = hcex_models_vbuf;
                        part->vertex_buffer.offset = source_vertex + vbuf_base;
                        part->triangle_buffer.offset = (int)part->triangles.address + ibuf_base;
                        part->triangle_buffer.hardware_format = hcex_models_ibuf;
                    }
                    while ( part_index < geometry->parts.count );
                }
                geometry_index = (short)(geometry_index + 1);
            }
            while ( geometry_index < model_tag->geometries.count );
        }
    }

    hcex_stat_vbuf_unlock(hcex_models_vbuf);
    hcex_stat_vbuf_unlock(hcex_models_ibuf);
}
