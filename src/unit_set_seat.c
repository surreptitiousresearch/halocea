/* unit_set_seat @0x836CF388 — thin wrapper that asks unit_set_or_test_seat_and_weapon_label to apply (test=1,
 * no weapon label) the named seat to the unit, returning whether the seat label matched. */

#include <stdint.h>

extern uint8_t unit_set_or_test_seat_and_weapon_label(int object_index, const char *seat_label, const char *weapon_label, uint8_t change_flag);

int unit_set_seat(int unit_index, const char *name)
{
    return unit_set_or_test_seat_and_weapon_label(unit_index, name, 0, 1) != 0;
}
