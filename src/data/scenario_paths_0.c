/* scenario_paths_0 @ 0x84174920 (.data, 40 bytes)
 * DB applied_types: char *scenario_paths_0[10];
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
 * data-init char *[10]. Campaign scenario tag paths.
 * Pointers recovered from the binary (big-endian); string targets read from .rdata.
 * Backslashes escaped 2026-07-30 (unescaped forms corrupted runtime bytes).
 */
char *scenario_paths_0[10] =
{
    "levels\\a10\\a10", /* 0 */
    "levels\\a30\\a30", /* 1 */
    "levels\\a50\\a50", /* 2 */
    "levels\\b30\\b30", /* 3 */
    "levels\\b40\\b40", /* 4 */
    "levels\\c10\\c10", /* 5 */
    "levels\\c20\\c20", /* 6 */
    "levels\\c40\\c40", /* 7 */
    "levels\\d20\\d20", /* 8 */
    "levels\\d40\\d40", /* 9 */
};
