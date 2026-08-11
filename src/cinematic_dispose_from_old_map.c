/* cinematic_dispose_from_old_map @ 0x83690EA8 — clear cinematic letterbox/progress flags */
#include "headers/cinematic_globals.h"
void cinematic_dispose_from_old_map(void)
{
    cinematic_globals->letter_box = 0;
    cinematic_globals->cinematic_in_progress = 0;
}
