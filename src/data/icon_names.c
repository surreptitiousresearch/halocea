/* icon_names @ 0x8417C868 (.data, 160 bytes)
 * DB applied_types: unsigned __int16 *icon_names[40];
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x8211CE0C -> "" (empty string at target)
 *   +0x0004 = 0x8211CDF8 -> "" (empty string at target)
 *   +0x0008 = 0x8211CDE4 -> "" (empty string at target)
 *   +0x000C = 0x8211CDD0 -> "" (empty string at target)
 *   +0x0010 = 0x8211CDB4 -> "" (empty string at target)
 *   +0x0014 = 0x8211CD98 -> "" (empty string at target)
 *   +0x0018 = 0x8211CD7C -> "" (empty string at target)
 *   +0x001C = 0x8211CD60 -> "" (empty string at target)
 *   +0x0020 = 0x8211CD50 -> "" (empty string at target)
 *   +0x0024 = 0x8211CD3C -> "" (empty string at target)
 *   +0x0028 = 0x8211CD28 -> "" (empty string at target)
 *   +0x002C = 0x8211CD10 -> "" (empty string at target)
 *   +0x0030 = 0x8211CCF4 -> "" (empty string at target)
 *   +0x0034 = 0x8211CCDC -> "" (empty string at target)
 *   +0x0038 = 0x8211CCC4 -> "" (empty string at target)
 *   +0x003C = 0x8211CCAC -> "" (empty string at target)
 *   +0x0040 = 0x8211CC94 -> "" (empty string at target)
 *   +0x0044 = 0x8211CC7C -> "" (empty string at target)
 *   +0x0048 = 0x8211CC6C -> "" (empty string at target)
 *   +0x004C = 0x8211CC50 -> "" (empty string at target)
 *   +0x0050 = 0x8211CC30 -> "" (empty string at target)
 *   +0x0054 = 0x8211CC0C -> "" (empty string at target)
 *   +0x0058 = 0x8211CC00 -> "" (empty string at target)
 *   +0x005C = 0x8211CBE4 -> "" (empty string at target)
 *   +0x0060 = 0x8211CBC4 -> "" (empty string at target)
 *   +0x0064 = 0x8211CBA4 -> "" (empty string at target)
 *   +0x0068 = 0x8211CB94 -> "" (empty string at target)
 *   +0x006C = 0x8211CB88 -> "" (empty string at target)
 *   +0x0070 = 0x8211CB78 -> "" (empty string at target)
 *   +0x0074 = 0x8211CB6C -> "" (empty string at target)
 *   +0x0078 = 0x8211CB60 -> "" (empty string at target)
 *   +0x007C = 0x8211CB54 -> "" (empty string at target)
 *   +0x0080 = 0x8211CB40 -> "" (empty string at target)
 *   +0x0084 = 0x8211CB2C -> "" (empty string at target)
 *   +0x0088 = 0x8211CB18 -> "" (empty string at target)
 *   +0x008C = 0x8211CB04 -> "" (empty string at target)
 *   +0x0090 = 0x8211CAF0 -> "" (empty string at target)
 *   +0x0094 = 0x8211CADC -> "" (empty string at target)
 *   +0x0098 = 0x8211CAC8 -> "" (empty string at target)
 *   +0x009C = 0x8211CAB4 -> "" (empty string at target)
 * unsigned __int16 *[40]; consumers use const wchar_t*. wchar_t is 16-bit on this target.
 */
#include <stddef.h>   /* wchar_t */

const wchar_t *icon_names[40] = {
    L"a-button", L"b-button", L"x-button", L"y-button",
    L"black-button", L"white-button", L"left-trigger", L"right-trigger",
    L"dpad-up", L"dpad-down", L"dpad-left", L"dpad-right",
    L"start-button", L"back-button", L"left-thumb", L"right-thumb",
    L"left-stick", L"right-stick", L"action", L"throw-grenade",
    L"primary-trigger", L"integrated-light", L"jump", L"use-equipment",
    L"rotate-weapons", L"rotate-grenades", L"crouch", L"zoom",
    L"accept", L"back", L"move", L"look",
    L"custom-1", L"custom-2", L"custom-3", L"custom-4",
    L"custom-5", L"custom-6", L"custom-7", L"custom-8",
};
