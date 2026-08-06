/* build_sprite_get_group @0x837EC1D0 — find or create the vertex-buffer group within `data` that batches
 * sprites for `bitmap`. Reuses an existing group if `bitmap` is already assigned to one; otherwise, if
 * there's room (fewer than 8 groups), claims the next free group, uploads the bitmap to the texture cache,
 * and allocates+locks a dynamic vertex buffer sized for `maximum_sprite_count` quads (type 6 normally, 8 if
 * flags bit 0 is set) — logging a one-time warning if the vertex buffer allocation fails. Returns the group
 * index, or -1 if the group table is full or the found/claimed group has no locked vertex buffer. */

#include <stdint.h>
#include "headers/build_sprite_data.h"
#include "headers/build_sprites_flags.h"
#include "headers/bitmap_data.h"
#include "headers/rasterizer_globals.h"
#include "headers/rasterizer_lock_operation.h"
#include "headers/blam_data_globals.h"


extern int _texture_cache_bitmap_get_hardware_format(bitmap_data *bitmap, uint8_t block, uint8_t load);
extern int rasterizer_dynamic_vertices_new(int16_t type, int count);
extern void * rasterizer_dynamic_vertices_lock(int dynamic_vertex_buffer_index);

int16_t build_sprite_get_group(build_sprite_data *data, bitmap_data *bitmap)
{
    int16_t index = 0;

    if ( data->group_count > 0 )
    {
        index = 0;
        do
        {
            if ( data->groups[index].bitmap == bitmap )
                break;
            index = (int16_t)(index + 1);
        } while ( index < data->group_count );
    }

    if ( index >= data->group_count )
    {
        if ( data->group_count < 8 )
        {
            if ( index >= data->group_count )
            {
                build_sprite_group *group = &data->groups[index];
                data->group_count++;
                group->bitmap = bitmap;

                if ( _texture_cache_bitmap_get_hardware_format(bitmap, 0, 1u) )
                {
                    rasterizer_globals.current_lock_operation = _rasterizer_lock_sprite;
                    int16_t type = (data->flags & (1u << _build_sprites_screen_space_bit)) == 0 ? 6 : 8;
                    group->vertex_buffer_index = rasterizer_dynamic_vertices_new(type, 4 * data->maximum_sprite_count);
                    if ( group->vertex_buffer_index == -1 )
                    {
                        if ( !warned_9 )
                            warned_9 = 1;
                        group->vertices = 0;
                        rasterizer_globals.current_lock_operation = 0;
                    }
                    else
                    {
                        group->vertices = rasterizer_dynamic_vertices_lock(group->vertex_buffer_index);
                        rasterizer_globals.current_lock_operation = 0;
                    }
                }
                else
                {
                    group->vertices = 0;
                    group->vertex_buffer_index = -1;
                }

                group->sprite_count = 0;
            }
        }
        else
        {
            index = -1;
        }
    }

    if ( index == -1 )
        return index;

    if ( data->groups[index].vertices )
        return index;

    return -1;
}
