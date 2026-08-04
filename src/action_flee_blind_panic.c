#include <stdint.h>
#include "headers/actor_panic_type.h"
/* action_flee_blind_panic @0x83825738 — true when panic_type falls in the "blind panic" range [9, 12]. */

uint8_t action_flee_blind_panic(int16_t panic_type)
{
    return panic_type >= _actor_panic_grenade_attached_to_us && panic_type <= _actor_panic_burning_to_death;
}
