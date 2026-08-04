/* unsigned __int16 *[40]; consumers use const wchar_t*. wchar_t is 16-bit on this target. */
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
