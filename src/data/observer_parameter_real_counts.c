/* observer_parameter_real_counts @ 0x84177CF8 (.data, 10 bytes)
 * DB applied_types: __int16 observer_parameter_real_counts[5];
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000: 0003 0003 0001 0001 0006
 * .data data-init (short[5]) = {3,3,1,1,6}.
 * Ledger size 12 overcounts (short[5]=10); trailing 2 bytes are a separate global.
 */
short observer_parameter_real_counts[5] = { 3, 3, 1, 1, 6 };
