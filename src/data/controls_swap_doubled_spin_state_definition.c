/* controls_swap_doubled_spin_state_definition @ 0x84180AF0 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include <stdint.h>
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern uint8_t controls_swap_doubled_spin_state;

hs_global_external controls_swap_doubled_spin_state_definition =
{
    "controls_swap_doubled_spin_state",
    hs_type_boolean,
    { 0, 0 },
    &controls_swap_doubled_spin_state,
    0
};
