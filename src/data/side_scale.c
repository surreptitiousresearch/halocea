/* side_scale @ 0x84176574 (.data, 4 bytes)
 * DB applied_types: float side_scale;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0xB851B717 -> -4.99999987e-05f
 * DEVIATION: the previous reconstruction was +4.999999873689376e-05f and its own comment recorded
 * the word as 0x3851B717. The image word is 0xB851B717 — the sign bit is SET, so the constant is
 * negative. A transcription that drops the sign bit inverts whatever this scales.
 * Caught 2026-08-06 by data_provenance.py --verify against the .data record. */
float side_scale = -4.999999873689376e-05f;
