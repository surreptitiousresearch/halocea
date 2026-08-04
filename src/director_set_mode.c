/* director_set_mode @ 0x836E4600 — switch the director's global game mode. When the mode actually
 * changes, latch the initialize_camera flag so the next update re-picks the camera. */

#include <stdint.h>
#include "headers/director.h"
#include "headers/blam_data_globals.h"

void director_set_mode(int16_t mode)
{
    if ( director_globals.game_mode != mode )
    {
        director_globals.game_mode = mode;
        director_globals.initialize_camera = 1;
    }
}
