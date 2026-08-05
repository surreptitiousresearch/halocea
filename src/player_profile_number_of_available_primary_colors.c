/* player_profile_number_of_available_primary_colors @0x83764F70 — retail stub: unconditionally returns
 * NUMBER_OF_PLAYER_PROFILE_COLORS. */

#include <stdint.h>
#include "headers/player_profile_colors.h"

uint16_t player_profile_number_of_available_primary_colors(void)
{
    return NUMBER_OF_PLAYER_PROFILE_COLORS;
}
