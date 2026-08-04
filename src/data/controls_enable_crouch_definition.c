/* controls_enable_crouch_definition @ 0x84180AC0 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned __int8 controls_enable_crouch;

hs_global_external controls_enable_crouch_definition =
{
    "controls_enable_crouch",
    hs_type_boolean,
    { 0, 0 },
    &controls_enable_crouch,
    0
};
