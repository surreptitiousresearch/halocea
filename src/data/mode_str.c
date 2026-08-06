/* mode_str @ 0x84184CA8 (.data, 8 bytes)
 * DB applied_types: const char *mode_str[2];
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x82126528 -> "flying camera"
 *   +0x0004 = 0x82126518 -> "orbiting camera"
 * .data init @0x84184CA8: pointers to "flying camera", "orbiting camera"
 */
const char *mode_str[2] = { "flying camera", "orbiting camera" };
