/* icon_is_special @ 0x8417C854 (.data, 18 bytes)
 * DB applied_types: unsigned __int8 icon_is_special[18];
 * Per-icon "is special" flag table.
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000: 00 00 00 00 00 00 01 01 00 00 00 00 00 00 01 01
 *   +0x0010: 01 01
 * DEVIATION: the previous reconstruction was shifted one slot left at the head and carried a
 * seventh set flag — it read {..,[5]=1,[6]=1,[7]=0,..,[13]=1,[14..17]=1} where the image has
 * {..,[6]=1,[7]=1,..,[14..17]=1}. Six icons are special, not seven, and they are 6, 7, 14-17.
 * Caught 2026-08-06 by data_provenance.py --verify against the .data record. */

const unsigned char icon_is_special[18] = { 0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1 };
