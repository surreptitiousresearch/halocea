/* bitmap_type_string_table @ 0x84176CF8 (.data, 20 bytes)
 * DB applied_types: const char *bitmap_type_string_table[5];
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x82113EB4 -> "2d texture"
 *   +0x0004 = 0x82113EA8 -> "3d texture"
 *   +0x0008 = 0x82113E9C -> "cube map"
 *   +0x000C = 0x00000000
 *   +0x0010 = 0x00000000
 * .data data-init (const char *[5]).
 * DB decl is [5]; ledger size 24 overcounts (5 pointers = 20 bytes).
 */
const char *bitmap_type_string_table[5] = {
    "2d texture",
    "3d texture",
    "cube map",
    0,
    0,
};
