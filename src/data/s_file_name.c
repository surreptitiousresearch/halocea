/* s_file_name @ 0x8417E930 (.data, 4 bytes)
 * DB applied_types: const char *s_file_name;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x8211DB24 -> "ServerPlayerUpdateHistory.log"
 * Blam .lib data-global storage definition (init batch 7).
 */
/* s_file_name @0x8417E930, DB extent 8. Binary stores a pointer (0x8211DB24) to the
 * string below + a trailing NULL word. Consumers extern it as `char s_file_name[]` but use
 * it as a filename in fopen(); the faithful storage is a char* (pointer), defined as such. */
char *s_file_name = "ServerPlayerUpdateHistory.log";
