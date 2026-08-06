/* display_precache_progress_definition @ 0x8417EF50 (.data, 16 bytes)
 * DB applied_types: hs_global_external display_precache_progress_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x82122160 -> "display_precache_progress"
 *   +0x04 type                       = 0x0005
 *   +0x08 pointer                    = 0x8441E42A -> display_precache_progress
 *   +0x0C console_flags              = 0x00
 * second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified.
 */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern unsigned char display_precache_progress;

hs_global_external display_precache_progress_definition =
{
    "display_precache_progress",  /* name */
    hs_type_boolean,              /* type */
    { 0, 0 },                     /* _pad06.._pad07 */
    &display_precache_progress,   /* pointer */
    0,                            /* console_flags */
};
