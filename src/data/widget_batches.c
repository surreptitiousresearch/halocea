/* widget_batches @ 0x8465E620 (.data, 430200 bytes; the image dump covers only the first 65536)
 * DB applied_types: widget_batch widget_batches[5];
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 .. +0xFFFF: all zero (65536 bytes, uninitialised storage)
 * widget_batches — UI widget render batches (data-bss); DB 5*86040=430200, ledger 430208 (next-gap overcount)
 */
#include "../headers/widget_batch.h"

widget_batch widget_batches[5];
