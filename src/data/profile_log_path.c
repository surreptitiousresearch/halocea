/* profile_log_path @ 0x84182818 (.data, 4 bytes)
 * DB applied_types: const char *profile_log_path;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x8212395C -> "d:\r-prof.txt"
 * Blam .lib data-global storage definition (init batch 7).
 */
/* profile_log_path @0x84182818, DB extent 4 = char * -> "d:\r-prof.txt". No current
 * consumers (state=absent); extern added to headers/profile_log_path.h. */
char *profile_log_path = "d:\\r-prof.txt";
