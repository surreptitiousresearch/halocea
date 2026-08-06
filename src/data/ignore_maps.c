/* ignore_maps @ 0x8417EDA8 (.data, 52 bytes)
 * DB applied_types: char *ignore_maps[13];
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x8210F368 -> "a10"
 *   +0x0004 = 0x8210F364 -> "a30"
 *   +0x0008 = 0x8210F360 -> "a50"
 *   +0x000C = 0x8210EE88 -> "b30"
 *   +0x0010 = 0x8210F35C -> "b40"
 *   +0x0014 = 0x8210EC38 -> "c10"
 *   +0x0018 = 0x8210EE48 -> "c20"
 *   +0x001C = 0x8210F358 -> "c40"
 *   +0x0020 = 0x8210EE6C -> "d20"
 *   +0x0024 = 0x8210EE94 -> "d40"
 *   +0x0028 = 0x82002384 -> "ui"
 *   +0x002C = 0x8211DAD8 -> "bitmaps"
 *   +0x0030 = 0x8207E46C -> "sounds"
 * Blam .lib data-global storage definition (init batch 7).
 */
/* ignore_maps @0x8417EDA8, DB extent 56 = const char *[14] (13 names + NULL terminator).
 * Consumer extern is const char *ignore_maps[13]; binary carries a trailing NULL sentinel. */
const char *ignore_maps[13] = {
    "a10","a30","a50","b30","b40","c10","c20","c40","d20","d40",
    "ui","bitmaps","sounds"
};
