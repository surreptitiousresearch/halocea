/* internal_set_texture @0x837860A0 */
#include <stdint.h>

extern uint8_t rasterizer_set_texture_direct_non_blocking(int16_t stage, int bitmap_group_index, int16_t bitmap_index);
extern int rasterizer_set_texture_non_blocking(int16_t stage, int16_t type, int16_t usage, int bitmap_group_index, int16_t bitmap_index);

uint8_t internal_set_texture(int16_t stage_index, int bitmap_group_index, int16_t sequence_index)
{
    if (bitmap_group_index == -1)
        return rasterizer_set_texture_direct_non_blocking(stage_index, -1, sequence_index);
    else
        return rasterizer_set_texture_non_blocking(stage_index, 0, 1, bitmap_group_index, sequence_index);
}
