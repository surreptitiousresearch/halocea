/* sizes @ 0x84184C24 (.data, 12 bytes)
 * DB applied_types: float sizes[3];
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x00000000
 *   +0x0004 = 0xBF400000
 *   +0x0008 = 0x3F800000
 * Storage definition for Blam data-segment global `sizes` (data-init).
 * float[3], big-endian bytes @0x84184C24 = {0.0, -0.75, 1.0}. Verified against binary.
 */
float sizes[3] = { 0.0f, -0.75f, 1.0f };
