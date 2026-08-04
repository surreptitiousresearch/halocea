/* client_log_destination_definition @ 0x8417F980 -- second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified. */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern int client_log_destination;

hs_global_external client_log_destination_definition =
{
    "client_log_destination",
    hs_type_long_integer,
    { 0, 0 },
    &client_log_destination,
    0
};
