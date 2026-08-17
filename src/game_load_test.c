/* game_load_test @0x83687A28 */
#include <stdint.h>

extern uint8_t cache_file_exists(const char *name);

uint8_t game_load_test(const char *map_name)
{
    return cache_file_exists(map_name);
}
