/* error_suppress_all_definition @ 0x84180BB0 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

#include "../headers/error_global_data.h"
extern error_global_data error_globals;

hs_global_external error_suppress_all_definition =
{
    "error_suppress_all",
    hs_type_boolean,
    { 0, 0 },
    (char *)&error_globals + 5,
    0
};
