/* developer_mode_definition @ 0x84180C10 (.data, 16 bytes)
 * DB applied_types: hs_global_external developer_mode_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x8211F014 -> "developer_mode"
 *   +0x04 type                       = 0x0007
 *   +0x08 pointer                    = 0x846C92E6 -> "" (empty string at target)
 *   +0x0C console_flags              = 0x00
 * second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified.
 */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

#include "../headers/error_global_data.h"
extern error_global_data error_globals;

hs_global_external developer_mode_definition =
{
    "developer_mode",            /* name */
    hs_type_short_integer,       /* type */
    { 0, 0 },                    /* _pad06.._pad07 */
    (char *)&error_globals + 6,  /* pointer */
    0,                           /* console_flags */
};
