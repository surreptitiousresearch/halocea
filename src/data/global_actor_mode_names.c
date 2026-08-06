/* global_actor_mode_names @ 0x84184FBC (.data, 16 bytes)
 * DB applied_types: const char *global_actor_mode_names[4];
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x82113340 -> "asleep"
 *   +0x0004 = 0x820DC020 -> "alert"
 *   +0x0008 = 0x82128678 -> "combat"
 *   +0x000C = 0x00000000
 * .data init @0x84184FBC: { "asleep", "alert", "combat", NULL }
 */
const char *global_actor_mode_names[4] = { "asleep", "alert", "combat", 0 };
