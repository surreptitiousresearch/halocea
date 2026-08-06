/* effect_marker_names @ 0x8417E93C (.data, 20 bytes)
 * DB applied_types: const char *effect_marker_names[5];
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x82001A9C -> "normal"
 *   +0x0004 = 0x82110F64 -> "incident"
 *   +0x0008 = 0x82110F50 -> "negative incident"
 *   +0x000C = 0x820BB660 -> "reflection"
 *   +0x0010 = 0x8205B92C -> "gravity"
 * Blam .lib data-global storage definition (init batch 7).
 */
/* effect_marker_names @0x8417E93C, DB extent 20 = const char *[5]. */
const char *effect_marker_names[] = {
    "normal", "incident", "negative incident", "reflection", "gravity"
};
