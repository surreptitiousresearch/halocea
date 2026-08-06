/* next_field @ 0x84177CE0 (.data, 6 bytes)
 * DB applied_types: __int16 next_field[3];
 * Cyclic successor table: next_field[i] == (i + 1) % 3. Used to walk the two remaining axes once
 * the dominant one is known — matrix4x3_rotation_to_quaternion.c:28-29 does j = next_field[i],
 * k = next_field[j].
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000: 0001 0002 0000
 * DEVIATION: the previous reconstruction was { 0, 1, 2 } — the identity, not the successor. Under
 * it j == i == k and the three-axis cycle in the matrix-to-quaternion conversion collapses onto a
 * single axis. The consumer's own file comment already stated {1,2,0}; only the definition was
 * wrong. Caught 2026-08-06 by data_provenance.py --verify against the .data record.
 * The predecessor comment also claimed "ledger size 8 overcounts": applied_types says __int16[3]
 * = 6 bytes and next_field_0 begins at 0x84177CE8, so the two are 8 apart with 2 bytes of
 * inter-symbol padding. */
const short next_field[3] = { 1, 2, 0 };
