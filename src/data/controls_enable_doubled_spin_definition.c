/* controls_enable_doubled_spin_definition @ 0x84180AE0 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned __int8 controls_enable_doubled_spin;

hs_global_external controls_enable_doubled_spin_definition =
{
    "controls_enable_doubled_spin",
    hs_type_boolean,
    { 0, 0 },
    &controls_enable_doubled_spin,
    0
};
