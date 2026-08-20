/* hs_find_function_by_name @0x8372F070 — return the index of a built-in HaloScript function by name, or -1.
 * Includes a backwards-compatibility alias: the legacy name "player_effect_set_max_rumble" is remapped to the
 * current "player_effect_set_max_vibrate" before the (case-insensitive, 471-entry) table search.
 *
 * DEVIATION: the image never stores the legacy name whole — it assembles it at run time in the 176-byte
 * stack buffer from two literals. memcpy(buf, "player_effect_set_max_ru", 0x19) @0x8372F094 (literal
 * @0x8211CA00, 24 chars + NUL) is followed by an inlined strlen/strcat of "mble" @0x8372F0A4-0x8372F0CC
 * (literal @0x8211C9F8 = 6D 62 6C 65 00), and stricmp(name, buf) @0x8372F0D8 therefore compares against the
 * full 28-character "player_effect_set_max_rumble". Reconstructed as strcpy + strcat: the compiler expands
 * strcpy of a 24-character literal into memcpy of 25 bytes and inlines strcat, so this is the same code, not
 * a simplification.
 *
 * A 2026-08-06 find_string_fidelity.py hit was adjudicated as a fabricated string and the pair was collapsed
 * to the 24-character fragment alone; that made the legacy alias unmatchable and is reverted here. The
 * fragment "mble" is five bytes, below the strings-table minimum length, which is why the string search could
 * not see the second half. (The reverted note also cited @0x8213E440 for the first fragment; that address
 * holds Havok configuration text — the fragment is at 0x8211CA00.) */

#include <stdint.h>
#include "headers/hs_function_definition.h"

extern char *strcpy(char *dst, const char *src);
extern char *strcat(char *dst, const char *src);
extern int stricmp(const char *a, const char *b);

#define HS_FUNCTION_COUNT 471

int16_t hs_find_function_by_name(const char *name)
{
    const char *lookup = name;

    char legacy_alias[176];
    strcpy(legacy_alias, "player_effect_set_max_ru");
    strcat(legacy_alias, "mble");
    if ( !stricmp(name, legacy_alias) )
        lookup = "player_effect_set_max_vibrate";

    for ( int16_t i = 0; i < HS_FUNCTION_COUNT; i = (int16_t)(i + 1) )
        if ( !stricmp(hs_function_table[i]->name, lookup) )
            return i;
    return -1;
}
