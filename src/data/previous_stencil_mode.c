/* previous_stencil_mode @ 0x84174970 (.data, 2 bytes)
 * DB applied_types: __int16 previous_stencil_mode;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = ffff (2 trailing bytes)
 * previous_stencil_mode (0x84174970). __int16, bytes FF FF = -1.
 * (ledger size 8 overcounts; following 00 00 belongs to a distinct global.)
 */
short previous_stencil_mode = -1;
