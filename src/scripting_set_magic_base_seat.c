#include <stdint.h>
#include "headers/blam_data_globals.h"
/* scripting_set_magic_base_seat @0x836CFE34 — script hook: resolve a seat label string to its base
 * seat index and store it as the "magic" base seat used by seat-relative scripting. */

extern int16_t seat_label_to_base_seat_index(const char *seat_label);

void scripting_set_magic_base_seat(const char *string)
{
    magic_base_animation_seat_index = seat_label_to_base_seat_index(string);
}
