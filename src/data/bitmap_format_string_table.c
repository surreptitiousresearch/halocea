/* bitmap_format_string_table @ 0x84176D10 — .data data-init (const char *[19]).
 * Empty-string slots point at the shared "" .rdata constant (0x8200155A). */
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
