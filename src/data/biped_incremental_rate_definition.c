/* biped_incremental_rate_definition @ 0x8417F8C0 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern int biped_incremental_rate;

hs_global_external biped_incremental_rate_definition =
{
    "biped_incremental_rate",
    hs_type_long_integer,
    { 0, 0 },
    &biped_incremental_rate,
    0
};
