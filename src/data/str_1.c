/* str_1 @ 0x84184CB0 (.data, 8 bytes)
 * DB applied_types: const char *str_1[2];
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x82126510 -> "exiting"
 *   +0x0004 = 0x82126504 -> "entering"
 * .data init @0x84184CB0: pointers to "exiting", "entering"
 */
const char *str_1[2] = { "exiting", "entering" };
