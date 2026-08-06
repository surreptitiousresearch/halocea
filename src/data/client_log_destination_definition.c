/* client_log_destination_definition @ 0x8417F980 (.data, 16 bytes)
 * DB applied_types: hs_global_external client_log_destination_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x82120F84 -> "client_log_destination"
 *   +0x04 type                       = 0x0008
 *   +0x08 pointer                    = 0x84469794 -> client_log_destination
 *   +0x0C console_flags              = 0x00
 * second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified.
 */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern int client_log_destination;

hs_global_external client_log_destination_definition =
{
    "client_log_destination",  /* name */
    hs_type_long_integer,      /* type */
    { 0, 0 },                  /* _pad06.._pad07 */
    &client_log_destination,   /* pointer */
    0,                         /* console_flags */
};
