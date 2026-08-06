/* global_scenario_index @ 0x84177C3C (.data, 4 bytes)
 * DB applied_types: int global_scenario_index;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0xFFFFFFFF
 * data-init int (4 bytes). Binary bytes 0xFFFFFFFF big-endian = -1
 * (the "no scenario loaded" sentinel).
 */
int global_scenario_index = -1;
