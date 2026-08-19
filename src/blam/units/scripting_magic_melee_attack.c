/* scripting_magic_melee_attack @0x836CFE00 — script command: make player 0's unit perform a melee attack.
 * Selects player 0's controlled unit via player_datum.unit_index (@0x34). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"

#include "headers/real_vector2d.h"
extern uint8_t unit_melee_attack_begin(int unit_index, uint8_t continuous, const real_vector2d *alignment_vector);

uint8_t scripting_magic_melee_attack(void)
{
    return unit_melee_attack_begin(DATA_ARRAY_ELEMENT(player_data, player_datum, 0)->unit_index, 0, 0);
}
