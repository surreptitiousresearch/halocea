/* transition_function_tables @ 0x844A5CC4 (.data, 24 bytes)
 * DB applied_types: unsigned __int8 *transition_function_tables[6];
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x00000000
 *   +0x0004 = 0x00000000
 *   +0x0008 = 0x00000000
 *   +0x000C = 0x00000000
 *   +0x0010 = 0x00000000
 *   +0x0014 = 0x00000000
 * data-bss, unsigned char*[6] = 24 bytes; ledger gap 28.
 * P4 sweep flagged lbz/stb at +0x18 (== array size): that is a 1-byte sibling global at 0x844A5CDC
 * (in the 4-byte gap before sequential_counter @0x844A5CE0), reached via a shared base register. If this
 * were transition_function_tables[7] the 7th slot would be a pointer (lwz), not a byte. Type [6] correct.
 */
unsigned char *transition_function_tables[6];
