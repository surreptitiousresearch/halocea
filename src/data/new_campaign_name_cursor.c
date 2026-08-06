/* new_campaign_name_cursor @ 0x8441E67A (.data, 2 bytes)
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0000 (2 trailing bytes)
 */
#include <stdint.h>
/* new_campaign_name_cursor @ 0x8441E67A - .data zero-fill (IDA word_8441E67A, no PDB name).
 * Halfword: new_campaign_chosen clears it with `sth r11,(word_8441E67A-0x8441E67D)(r31)` (disasm 0x836975D4),
 * r31 anchored on the DB-named new_campaign_begin_editing (0x8441E67D). */

int16_t new_campaign_name_cursor;
