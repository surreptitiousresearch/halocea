/* hcex_arifle_ammo @ 0x84172058 (.data, 8 bytes)
 * DB applied_types: int hcex_arifle_ammo[2];
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0xFFFFFFFF
 *   +0x0004 = 0xFFFFFFFF
 * 8 bytes (.data, init 0xFFFFFFFF x2). Per-local-player
 *    shadow of assault-rifle rounds_loaded for HUD updates; -1 = uninitialized.
 */
int hcex_arifle_ammo[2] = { -1, -1 };
