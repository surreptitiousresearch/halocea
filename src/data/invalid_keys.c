/* invalid_keys @ 0x84181C00 (.data, 4 bytes)
 * DB applied_types: const char *invalid_keys;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x82123510 -> "*\/\|,.;:"^&?<>"
 * Blam .lib data-global storage definition (init batch 7).
 */
/* invalid_keys @0x84181C00, DB extent 4 = const char * -> virtual-keyboard blacklist.
 * Final char is byte 0xA0 (non-breaking space), emitted as octal \240. */
const char *invalid_keys = "*/\\|,.;:\"^&?<>\240";

