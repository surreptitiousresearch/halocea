/* s_file_name_0 @ 0x8417EEC8 (.data, 4 bytes)
 * DB applied_types: const char *s_file_name_0;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x8211EA5C -> "ClientPlayerUpdateHistory.log"
 * .data init: a pointer word holding the .rdata address 0x8211EA5C (DB `strings` row there:
 * ascii, length 30 = 29 characters + terminator). The four bytes 0x8417EECC..0x8417EECF are
 * zero alignment padding ahead of screenshot_size_definition (hs_global_external, 16 bytes,
 * 16-byte aligned at 0x8417EED0) — not part of this object.
 * Consumer: player_update_client_log @0x8376DAA8 does `lis r11, s_file_name_0@ha` /
 * `lwz r3, s_file_name_0@l(r11)` @0x8376DB40 and passes r3 straight to fopen(..., "a") — a
 * LOAD of the word, not an address-of, so the storage is a pointer and not a char array.
 * Client twin of s_file_name @0x8417E930 ("ServerPlayerUpdateHistory.log", src/data/s_file_name.c).
 */
const char *s_file_name_0 = "ClientPlayerUpdateHistory.log";
