/* equipment_incremental_rate_definition @ 0x8417EFF0 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern int equipment_incremental_rate;

hs_global_external equipment_incremental_rate_definition =
{
    "equipment_incremental_rate",
    hs_type_long_integer,
    { 0, 0 },
    &equipment_incremental_rate,
    0
};
