/* cheat_controller_definition @ 0x8417F7B0 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

#include "../headers/cheats.h"
extern cheat_globals cheat;

hs_global_external cheat_controller_definition =
{
    "cheat_controller",
    hs_type_boolean,
    { 0, 0 },
    (char *)&cheat + 8,
    0
};
