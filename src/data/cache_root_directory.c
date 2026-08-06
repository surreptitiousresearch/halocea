/* cache_root_directory @ 0x84176DE8 (.data, 256 bytes)
 * DB applied_types: char cache_root_directory[256];
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000: 64 3A 5C 00 00 00 00 00 00 00 00 00 00 00 00 00
 *   +0x0010: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 *   +0x0020: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 *   +0x0030: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 *   +0x0040: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 *   +0x0050: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 *   +0x0060: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 *   +0x0070: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 *   +0x0080: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 *   +0x0090: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 *   +0x00A0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 *   +0x00B0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 *   +0x00C0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 *   +0x00D0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 *   +0x00E0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 *   +0x00F0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 * .data data-init.
 * DB type char[256]; bytes begin 64 3A 5C 00 ("d:\") then zero-fill.
 * NOTE: several consumers inline-extern this as `char *cache_root_directory`, but the
 * binary storage is a 256-byte char array (the leading bytes are the ASCII path "d:\",
 * not a pointer). The array form is the ABI-correct definition.
 */
char cache_root_directory[256] = "d:\\";
