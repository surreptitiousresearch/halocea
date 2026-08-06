/* thread_pool @ 0x844A6208 (.data, 256 bytes)
 * DB applied_types: thread thread_pool[32];
 * Image bytes (big-endian), decoded from the binary .data record:
 *   [ 0] +0x00 object                     = 0x00000000
 *        +0x04 in_use                     = 0x00
 *   [ 1] +0x00 object                     = 0x00000000
 *        +0x04 in_use                     = 0x00
 *   [ 2] +0x00 object                     = 0x00000000
 *        +0x04 in_use                     = 0x00
 *   [ 3] +0x00 object                     = 0x00000000
 *        +0x04 in_use                     = 0x00
 *   [ 4] +0x00 object                     = 0x00000000
 *        +0x04 in_use                     = 0x00
 *   [ 5] +0x00 object                     = 0x00000000
 *        +0x04 in_use                     = 0x00
 *   [ 6] +0x00 object                     = 0x00000000
 *        +0x04 in_use                     = 0x00
 *   [ 7] +0x00 object                     = 0x00000000
 *        +0x04 in_use                     = 0x00
 *   [ 8] +0x00 object                     = 0x00000000
 *        +0x04 in_use                     = 0x00
 *   [ 9] +0x00 object                     = 0x00000000
 *        +0x04 in_use                     = 0x00
 *   [10] +0x00 object                     = 0x00000000
 *        +0x04 in_use                     = 0x00
 *   [11] +0x00 object                     = 0x00000000
 *        +0x04 in_use                     = 0x00
 *   [12] +0x00 object                     = 0x00000000
 *        +0x04 in_use                     = 0x00
 *   [13] +0x00 object                     = 0x00000000
 *        +0x04 in_use                     = 0x00
 *   [14] +0x00 object                     = 0x00000000
 *        +0x04 in_use                     = 0x00
 *   [15] +0x00 object                     = 0x00000000
 *        +0x04 in_use                     = 0x00
 *   [16] +0x00 object                     = 0x00000000
 *        +0x04 in_use                     = 0x00
 *   [17] +0x00 object                     = 0x00000000
 *        +0x04 in_use                     = 0x00
 *   [18] +0x00 object                     = 0x00000000
 *        +0x04 in_use                     = 0x00
 *   [19] +0x00 object                     = 0x00000000
 *        +0x04 in_use                     = 0x00
 *   ... 12 further elements elided; full hex in .sweep/data_image.tsv
 * data-bss zero-fill; DB/consumer thread[32] (256).
 * Consumers spell the bound THREAD_POOL_CAPACITY (== 32); literal used for standalone compile.
 */
#include "../headers/thread.h"

thread thread_pool[32];
