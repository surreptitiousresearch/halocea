/* player_effect_get @0x837374C0 */
#include <stdint.h>
#include "headers/player_effect_globals_definition.h"

player_effect_globals_definition * player_effect_get(int16_t local_player_index)
{
    /* recovered: (char *)player_effect_globals + 236 * local_player_index -> local_player_effect_data[i]
     * (stride 236 == sizeof(player_effect_datum)); return type kept per DB prototype */
    return (player_effect_globals_definition *)
        &player_effect_globals->local_player_effect_data[local_player_index];
}
