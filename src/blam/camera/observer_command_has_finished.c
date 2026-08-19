/* observer_command_has_finished @0x8370EAE8 */
#include <stdint.h>
#include "headers/observer_globals.h"

int observer_command_has_finished(int16_t local_player_index)
{
    observer *player = &observer_globals.local_players[local_player_index];

    if ( player->last_command.timer == 0.0f )
    {
        for ( int i = 0; i < 5; i++ )
        {
            if ( player->last_command.parameter_timers[i] != 0.0f )
                return 0;
        }
        return 1;
    }
    return 0;
}
