/* next_field_0 @ 0x84177CE8 (.data, 6 bytes)
 * DB applied_types: __int16 next_field_0[3];
 * ICF twin of next_field @0x84177CE0 — same cyclic successor table, (i + 1) % 3, kept as a
 * separate object by the linker.
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000: 0001 0002 0000
 * DEVIATION: the previous reconstruction was { 0, 1, 2 } — the identity, not the successor; see
 * next_field.c for the full adjudication. Caught 2026-08-06 by data_provenance.py --verify. */
const short next_field_0[3] = { 1, 2, 0 };
