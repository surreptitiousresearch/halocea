/* hs_find_function_by_name @0x8372F070 — return the index of a built-in HaloScript function by name, or -1.
 * Includes a backwards-compatibility alias: the legacy name "player_effect_set_max_rumble" is remapped to the
 * current "player_effect_set_max_vibrate" before the (case-insensitive, 471-entry) table search.
 *
 * Deviation: the decompiler builds the legacy alias by concatenating two literal fragments
 * ("player_effect_set_max_ru" + "mble"); reconstructed here as the single literal it forms. */

#include <stdint.h>
#include "headers/hs_function_definition.h"

extern char *strcpy(char *dst, const char *src);
extern int stricmp(const char *a, const char *b);

#define HS_FUNCTION_COUNT 471

int16_t hs_find_function_by_name(const char *name)
{
    const char *lookup = name;

    char legacy_alias[176];
    /* DEVIATION: the alias was transcribed as "player_effect_set_max_rumble". The binary's string
     * is "player_effect_set_max_ru" @0x8213E440 — 24 characters, NUL-terminated (strings.length
     * 25, and the following bytes are 00 00 00 00 then the next literal), and no string containing
     * "rumble" exists anywhere in the image. Four invented characters made this comparison
     * unmatchable, so the legacy name never resolved to player_effect_set_max_vibrate.
     * Found 2026-08-06 by tools/find_string_fidelity.py. */
    strcpy(legacy_alias, "player_effect_set_max_ru");
    if ( !stricmp(name, legacy_alias) )
        lookup = "player_effect_set_max_vibrate";

    for ( int16_t i = 0; i < HS_FUNCTION_COUNT; i = (int16_t)(i + 1) )
        if ( !stricmp(hs_function_table[i]->name, lookup) )
            return i;
    return -1;
}
