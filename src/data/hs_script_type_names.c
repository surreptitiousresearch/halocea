/* hs_script_type_names @ 0x8417802C (.data, 20 bytes)
 * DB applied_types: const char *hs_script_type_names[5];
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x82118CE8 -> "startup"
 *   +0x0004 = 0x82118CE0 -> "dormant"
 *   +0x0008 = 0x82118CD4 -> "continuous"
 *   +0x000C = 0x82118CCC -> "static"
 *   +0x0010 = 0x820F2FA8 -> "stub"
 * hs_script_type_names (0x8417802C). const char*[5].
 */
const char *hs_script_type_names[5] = {
    "startup",
    "dormant",
    "continuous",
    "static",
    "stub",
};
