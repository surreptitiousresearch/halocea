/* error_suppress_all_definition @ 0x84180BB0 (.data, 16 bytes)
 * DB applied_types: hs_global_external error_suppress_all_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x8211F0A4 -> "error_suppress_all"
 *   +0x04 type                       = 0x0005
 *   +0x08 pointer                    = 0x846C92E5 -> "" (empty string at target)
 *   +0x0C console_flags              = 0x00
 * second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified.
 */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

#include "../headers/error_global_data.h"
extern error_global_data error_globals;

hs_global_external error_suppress_all_definition =
{
    "error_suppress_all",        /* name */
    hs_type_boolean,             /* type */
    { 0, 0 },                    /* _pad06.._pad07 */
    (char *)&error_globals + 5,  /* pointer */
    0,                           /* console_flags */
};
