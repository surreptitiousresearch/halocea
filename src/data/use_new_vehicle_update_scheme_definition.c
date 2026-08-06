/* use_new_vehicle_update_scheme_definition @ 0x8417F960 (.data, 16 bytes)
 * DB applied_types: hs_global_external use_new_vehicle_update_scheme_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x82120FC0 -> "use_new_vehicle_update_scheme"
 *   +0x04 type                       = 0x0005
 *   +0x08 pointer                    = 0x8417E225 -> use_new_vehicle_update_scheme
 *   +0x0C console_flags              = 0x00
 * second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified.
 */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned char use_new_vehicle_update_scheme;

hs_global_external use_new_vehicle_update_scheme_definition =
{
    "use_new_vehicle_update_scheme",  /* name */
    hs_type_boolean,                  /* type */
    { 0, 0 },                         /* _pad06.._pad07 */
    &use_new_vehicle_update_scheme,   /* pointer */
    0,                                /* console_flags */
};
