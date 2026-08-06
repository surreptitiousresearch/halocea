/* global_difficulty_level @ 0x841720B8 (.data, 2 bytes)
 * DB applied_types: __int16 global_difficulty_level;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0001 (2 trailing bytes)
 */
#include <stdint.h>
/* bytes: 0001 */
int16_t global_difficulty_level = 1;
