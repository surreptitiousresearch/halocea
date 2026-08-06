/* wtPrimary @ 0x84184D00 (.data, 12 bytes)
 * DB applied_types: float wtPrimary[3];
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x3DA7EF9E
 *   +0x0004 = 0x3F1C01A3
 *   +0x0008 = 0x3E9E00D2
 * .data data-init.
 * DB type float[3] (12 bytes); RGB->luminance primary weights.
 * Bytes: 3D A7 EF 9E, 3F 1C 01 A3, 3E 9E 00 D2 = {0.082, 0.6094, 0.3086}.
 * NOTE: consumers inline-extern this as `FCOLOR wtPrimary`, but FCOLOR is float[4]
 * (16 bytes) — the storage is exactly 12 bytes (float[3]) per the DB, so the array
 * form is the ABI-correct definition.
 */
float wtPrimary[3] = { 0.082f, 0.6094f, 0.3086f };
