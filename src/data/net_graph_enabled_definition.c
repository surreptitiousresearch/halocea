/* net_graph_enabled_definition @ 0x8417EFB0 (.data, 16 bytes)
 * DB applied_types: hs_global_external net_graph_enabled_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x821220E8 -> "net_graph_enabled"
 *   +0x04 type                       = 0x0005
 *   +0x08 pointer                    = 0x84464CA0 -> gNetGraphEnabled
 *   +0x0C console_flags              = 0x00
 * second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified.
 */
#include <stdint.h>
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern uint8_t gNetGraphEnabled;

hs_global_external net_graph_enabled_definition =
{
    "net_graph_enabled",  /* name */
    hs_type_boolean,      /* type */
    { 0, 0 },             /* _pad06.._pad07 */
    &gNetGraphEnabled,    /* pointer */
    0,                    /* console_flags */
};
