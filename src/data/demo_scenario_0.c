/* demo_scenario_0 @ 0x84174948 (.data, 4 bytes)
 * DB applied_types: char *demo_scenario_0;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x8210F36C -> "levels\b30\demob30"
 * demo_scenario_0 (0x84174948). char*, points to .rdata "levels\b30\demob30".
 * Backslashes escaped 2026-07-30: "\b" was compiling to a backspace byte.
 */
char *demo_scenario_0 = "levels\\b30\\demob30";
