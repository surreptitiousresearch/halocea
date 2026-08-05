#include <stdint.h>
#include "headers/widget_globals.h"
#include "headers/blam_data_globals.h"


uint16_t * widget_controller(widget_instance *widget)
{
    uint16_t letter;

    if ( (unsigned int)(widget->local_player_index + 1) > 4 )
    {
        letter = '?';
    }
    else if ( widget->local_player_index == 0 )
    {
        letter = '1';
    }
    else if ( widget->local_player_index == 1 )
    {
        letter = '2';
    }
    else if ( widget->local_player_index == 2 )
    {
        letter = '3';
    }
    else if ( widget->local_player_index == -1 )
    {
        letter = '1';
    }
    else
    {
        letter = '4';
    }

    result_1[0] = letter;
    result_1[1] = 0;
    return result_1;
}
