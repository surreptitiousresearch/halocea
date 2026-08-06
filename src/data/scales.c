/* scales @ 0x84184C60 (.data, 12 bytes)
 * DB applied_types: float scales[3];
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x3F000000
 *   +0x0004 = 0x3E800000
 *   +0x0008 = 0x3F800000
 * .data init @0x84184C60: {3F000000,3E800000,3F800000}
 */
float scales[3] = { 0.5f, 0.25f, 1.0f };
