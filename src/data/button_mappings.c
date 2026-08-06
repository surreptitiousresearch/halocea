/* button_mappings @ 0x84175900 (.data, 14 bytes)
 * DB applied_types: char button_mappings[14];
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000: 02 06 07 05 00 04 03 01 0B 0A 00 01 10 11
 * button_mappings (0x84175900). unsigned char[14], game-control -> button-index remap table.
 * bytes 02 06 07 05 00 04 03 01 0B 0A 00 01 10 11. (ledger size 16 overcounts.)
 */
unsigned char button_mappings[14] = { 2, 6, 7, 5, 0, 4, 3, 1, 11, 10, 0, 1, 16, 17 };
