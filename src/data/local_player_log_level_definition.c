/* local_player_log_level_definition @ 0x8417F990 (.data, 16 bytes)
 * DB applied_types: hs_global_external local_player_log_level_definition;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x82120F6C -> "local_player_log_level"
 *   +0x04 type                       = 0x0008
 *   +0x08 pointer                    = 0x84469798 -> local_player_log_level
 *   +0x0C console_flags              = 0x00
 * second-order .data; hs_global_external (16B, big-endian) reconstructed
 * from binary bytes. Pointed to by hs_external_globals[]. name/type DB-verified.
 */
#include "../headers/hs_global_external.h"
#include "../headers/hs_type.h"

extern int local_player_log_level;

hs_global_external local_player_log_level_definition =
{
    "local_player_log_level",  /* name */
    hs_type_long_integer,      /* type */
    { 0, 0 },                  /* _pad06.._pad07 */
    &local_player_log_level,   /* pointer */
    0,                         /* console_flags */
};
