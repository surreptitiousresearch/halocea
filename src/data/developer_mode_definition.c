/* developer_mode_definition @ 0x84180C10 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

#include "../headers/error_global_data.h"
extern error_global_data error_globals;

hs_global_external developer_mode_definition =
{
    "developer_mode",
    hs_type_short_integer,
    { 0, 0 },
    (char *)&error_globals + 6,
    0
};
