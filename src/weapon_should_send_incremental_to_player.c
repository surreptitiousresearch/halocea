#include <stdint.h>
/* weapon_should_send_incremental_to_player @0x836DAD20 — weapons always send incremental updates to
 * every player. */

uint8_t weapon_should_send_incremental_to_player(int object_index, int player_index)
{
    return 1;
}
