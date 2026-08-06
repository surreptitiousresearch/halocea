/* global_current_render_target @ 0x84176820 (.data, 2 bytes)
 * DB applied_types: __int16 global_current_render_target;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = ffff (2 trailing bytes)
 * .data data-init.
 * DB type __int16 (2 bytes); ledger size 8 overcounts. Bytes FF FF = -1 (NONE).
 */
short global_current_render_target = -1;
