/* alternative_directories @ 0x84176EE8 (.data, 24 bytes)
 * DB applied_types: char *alternative_directories[6];
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x82114074 -> "maps_de\"
 *   +0x0004 = 0x82114068 -> "maps_fr\"
 *   +0x0008 = 0x8211405C -> "maps_es\"
 *   +0x000C = 0x82114050 -> "maps_it\"
 *   +0x0010 = 0x82114048 -> "maps\"
 *   +0x0014 = 0x00000000
 * .data data-init (char *[6]).
 * Localized maps subdirectory search list; last slot NULL.
 */
char *alternative_directories[6] = {
    "maps_de\\",
    "maps_fr\\",
    "maps_es\\",
    "maps_it\\",
    "maps\\",
    0,
};
