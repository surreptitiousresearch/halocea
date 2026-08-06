/* scenario_paths @ 0x841720C4 (.data, 40 bytes)
 * DB applied_types: const char *scenario_paths[10];
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x8210F400 -> "levels\a10\a10"
 *   +0x0004 = 0x8210F3F0 -> "levels\a30\a30"
 *   +0x0008 = 0x8210EEB0 -> "levels\a50\a50"
 *   +0x000C = 0x8210F3E0 -> "levels\b30\b30"
 *   +0x0010 = 0x8210F3D0 -> "levels\b40\b40"
 *   +0x0014 = 0x8210F3C0 -> "levels\c10\c10"
 *   +0x0018 = 0x8210F3B0 -> "levels\c20\c20"
 *   +0x001C = 0x8210F3A0 -> "levels\c40\c40"
 *   +0x0020 = 0x8210F390 -> "levels\d20\d20"
 *   +0x0024 = 0x8210F380 -> "levels\d40\d40"
 * scenario_paths (0x841720C4). const char*[10], campaign level tag paths.
 */
/* Backslashes escaped 2026-07-30: unescaped forms compiled but corrupted the
 * runtime bytes (\a=BEL, \b=BS silently; \c,\d dropped the backslash). DB
 * strings 0x8210EE90..0x8210F3E0 verified. */
const char *scenario_paths[10] = {
    "levels\\a10\\a10",
    "levels\\a30\\a30",
    "levels\\a50\\a50",
    "levels\\b30\\b30",
    "levels\\b40\\b40",
    "levels\\c10\\c10",
    "levels\\c20\\c20",
    "levels\\c40\\c40",
    "levels\\d20\\d20",
    "levels\\d40\\d40",
};
