/* controls_swap_doubled_spin_state_definition @ 0x84180AF0 (.data, 16 bytes)
 * DB applied_types: hs_global_external controls_swap_doubled_spin_state_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x8211F1AC -> "controls_swap_doubled_spin_state"
 *   +0x04 type                       = 0x0005
 *   +0x08 pointer                    = 0x84429FF4 -> controls_swap_doubled_spin_state
 *   +0x0C console_flags              = 0x00
 * second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified.
 */
#include <stdint.h>
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern uint8_t controls_swap_doubled_spin_state;

hs_global_external controls_swap_doubled_spin_state_definition =
{
    "controls_swap_doubled_spin_state",  /* name */
    hs_type_boolean,                     /* type */
    { 0, 0 },                            /* _pad06.._pad07 */
    &controls_swap_doubled_spin_state,   /* pointer */
    0,                                   /* console_flags */
};
