/* bitmap_format_string_table @ 0x84176D10 (.data, 76 bytes)
 * DB applied_types: const char *bitmap_format_string_table[19];
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x8207B330 -> "alpha"
 *   +0x0004 = 0x820332A0 -> "intensity"
 *   +0x0008 = 0x82113E80 -> "combined alpha-intensity"
 *   +0x000C = 0x82113E64 -> "separate alpha-intensity"
 *   +0x0010 = 0x8200155A -> "" (empty string at target)
 *   +0x0014 = 0x8200155A -> "" (empty string at target)
 *   +0x0018 = 0x82113E58 -> "high-color"
 *   +0x001C = 0x82113E50 -> "r6g5b5"
 *   +0x0020 = 0x82113E34 -> "high-color with 1-bit alpha"
 *   +0x0024 = 0x82113E1C -> "high-color with alpha"
 *   +0x0028 = 0x82113E10 -> "true-color"
 *   +0x002C = 0x82113DF8 -> "true-color with alpha"
 *   +0x0030 = 0x8200155A -> "" (empty string at target)
 *   +0x0034 = 0x8200155A -> "" (empty string at target)
 *   +0x0038 = 0x82113DD0 -> "compressed with color-key transparency"
 *   +0x003C = 0x82113DB0 -> "compressed with explicit alpha"
 *   +0x0040 = 0x82113D8C -> "compressed with interpolated alpha"
 *   +0x0044 = 0x82113D78 -> "palettized bump map"
 *   +0x0048 = 0x00000000
 * .data data-init (const char *[19]).
 * Empty-string slots point at the shared "" .rdata constant (0x8200155A).
 */
const char *bitmap_format_string_table[19] = {
    "alpha",
    "intensity",
    "combined alpha-intensity",
    "separate alpha-intensity",
    "",
    "",
    "high-color",
    "r6g5b5",
    "high-color with 1-bit alpha",
    "high-color with alpha",
    "true-color",
    "true-color with alpha",
    "",
    "",
    "compressed with color-key transparency",
    "compressed with explicit alpha",
    "compressed with interpolated alpha",
    "palettized bump map",
    0,
};
