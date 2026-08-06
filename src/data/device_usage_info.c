/* device_usage_info @ 0x846DB478 (.data, 4 bytes)
 * DB applied_types: device_usage_info_s device_usage_info;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 last_used_index            = 0x00000000
 * data-bss zero-fill; DB device_usage_info_s (4).
 * ledger size 8 = next-gap overcount; DB struct is a single int (last_used_index).
 */
#include "../headers/input_abstraction_globals.h"

struct device_usage_info_s device_usage_info;
