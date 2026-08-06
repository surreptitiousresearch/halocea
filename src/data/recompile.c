/* recompile @ 0x8445F696 (.data, 1 bytes)
 * DB applied_types: unsigned __int8 recompile;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 00 (1 trailing byte)
 * data-bss. DB type unsigned __int8 (1); ledger gap 18 overcounts.
 * P4 sweep flagged word/half accesses at +0x2/+0x6/+0xA/+0xE: those are UNNAMED sibling globals packed
 * between recompile (1 byte, at odd address 0x...696) and hs_syntax_data_allocated (0x8445F6A8), reached
 * via a shared small-data base register. Sibling-block access, not overflow. Type unsigned char correct.
 */
unsigned char recompile;
