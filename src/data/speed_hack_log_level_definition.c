/* speed_hack_log_level_definition @ 0x8417F9D0 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern int speed_hack_log_level;

hs_global_external speed_hack_log_level_definition =
{
    "speed_hack_log_level",
    hs_type_long_integer,
    { 0, 0 },
    &speed_hack_log_level,
    0
};
