/* speed_hack_detection_definition @ 0x8417F9C0 (.data, 16 bytes)
 * DB applied_types: hs_global_external speed_hack_detection_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x82120F30 -> "speed_hack_detection"
 *   +0x04 type                       = 0x0005
 *   +0x08 pointer                    = 0x841857C0 -> speed_hack_detection
 *   +0x0C console_flags              = 0x00
 * second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified.
 */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned char speed_hack_detection;

hs_global_external speed_hack_detection_definition =
{
    "speed_hack_detection",  /* name */
    hs_type_boolean,         /* type */
    { 0, 0 },                /* _pad06.._pad07 */
    &speed_hack_detection,   /* pointer */
    0,                       /* console_flags */
};
