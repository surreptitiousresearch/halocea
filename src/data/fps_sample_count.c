/* fps_sample_count @ 0x8446F834 (.data, 2 bytes)
 * DB applied_types: __int16 fps_sample_count;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0000 (2 trailing bytes)
 */
#include <stdint.h>
/* fps_sample_count @ 0x8446F834 - data-bss zero-fill; DB __int16 (2). ledger size 4 = next-gap overcount. */

int16_t fps_sample_count;
