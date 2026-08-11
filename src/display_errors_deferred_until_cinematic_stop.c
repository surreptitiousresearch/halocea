/* display_errors_deferred_until_cinematic_stop @0x83736BA8 — flush the queue of error messages that
 * were deferred while a cinematic played: display each valid entry (error code < NUMBER_OF_ERROR_CODES) for both local
 * players, then clear the slot. */

#include <stdint.h>
#include "headers/widget_globals.h"
#include "headers/error_type.h"

extern void display_error(int16_t error_code, int16_t local_player_index, uint8_t modal, uint8_t pause_game_time);

void display_errors_deferred_until_cinematic_stop(void)
{
    for ( short local_player_index = 0; local_player_index < 2; ++local_player_index )
    {
        short error_code = widget_globals.deferred_cinematic_error[local_player_index].error_code;
        if ( (unsigned int)error_code < NUMBER_OF_ERROR_CODES )
            display_error(error_code,
                          local_player_index,
                          widget_globals.deferred_cinematic_error[local_player_index].modal,
                          widget_globals.deferred_cinematic_error[local_player_index].pause_game_time);
        widget_globals.deferred_cinematic_error[local_player_index].error_code = -1;
    }
}
