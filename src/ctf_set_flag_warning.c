#include <stdint.h>
#include "headers/ctf_globals.h"

void ctf_set_flag_warning(int team_index, uint8_t warning)
{
    ctf_globals.flag_warning[team_index] = warning;
    ctf_globals.flag_warning_time_in_ticks[team_index] = 0;
}
