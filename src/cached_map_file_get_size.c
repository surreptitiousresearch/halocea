/* cached_map_file_get_size @0x83753EB8 — fixed cache-file byte budget per map-file slot: 384MB for slots 0-1,
 * 35MB for slot 2, 128MB for any higher slot. */
#include <stdint.h>

int cached_map_file_get_size(int16_t map_file_index)
{
    if (map_file_index <= 1)
        return 402653184;
    if (map_file_index > 2)
        return 0x8000000;
    return 36700160;
}
