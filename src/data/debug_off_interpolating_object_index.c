/* debug_off_interpolating_object_index @ 0x84176818 (.data, 4 bytes)
 * DB applied_types: int debug_off_interpolating_object_index;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0xFFFFFFFF
 * .data data-init (int).
 * Bytes FF FF FF FF = -1. Ledger size 8 overcounts by 4 (trailing 00000000 is a
 * separate adjacent global); DB applied type is int (4 bytes).
 */
int debug_off_interpolating_object_index = -1;
