/* g_protocol_changeover_enabled @ 0x844AB850 (.data, 1 bytes)
 * DB applied_types: unsigned __int8 g_protocol_changeover_enabled;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 00 (1 trailing byte)
 * data-bss, 1 byte.
 * DB applied-type is `unsigned __int8` (size 1) — authoritative over the wider
 * 4-byte int view some consumers formerly carried (that view would overrun the
 * 1-byte storage). Consumers now normalized to the 1-byte width. Defined at the
 * DB-authoritative width.
 */
unsigned char g_protocol_changeover_enabled;
