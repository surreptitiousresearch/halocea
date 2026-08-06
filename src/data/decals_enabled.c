/* decals_enabled @ 0x8417D408 (.data, 1 bytes)
 * DB applied_types: unsigned __int8 decals_enabled;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 01 (1 trailing byte)
 * Blam .lib data-global storage definition (init batch 7).
 */
/* decals_enabled @0x8417D408, DB extent 1, value 0x01. Consumer inline extern says `int`
 * (over-wide); real storage is a single boolean byte, defined exact-size to match the binary. */
unsigned char decals_enabled = 1;
