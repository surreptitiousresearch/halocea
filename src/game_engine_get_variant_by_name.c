/* game_engine_get_variant_by_name @ 0x83751648
 *
 * Look up a built-in multiplayer game variant by its ASCII name. First tries the
 * hard-coded table of engine presets (each name maps to a build_game_variant_* factory);
 * if none match, falls back to enumerating the local player's available playlist profiles
 * and matching the name against each profile's human-readable description (wide compare).
 *
 * All build_game_variant_* factories are re-source siblings (extern boundary). Each returns
 * a pointer to the game_variant it was handed (variant_scratch), which is then copied through
 * working_variant into the caller's buffer, matching the original two-step memcpy.
 */

#include <stdint.h>
#include <string.h>
#include "headers/game_variant.h"

/* Built-in variant factories (siblings) — fill the passed game_variant and return it. */
extern game_variant * build_game_variant_slayer(game_variant *result);
extern game_variant * build_game_variant_slayer_pro(game_variant *result);
extern game_variant * build_game_variant_elimination(game_variant *result);
extern game_variant * build_game_variant_phantoms(game_variant *result);
extern game_variant * build_game_variant_endurance(game_variant *result);
extern game_variant * build_game_variant_rockets(game_variant *result);
extern game_variant * build_game_variant_snipers(game_variant *result);
extern game_variant * build_game_variant_team_slayer(game_variant *result);
extern game_variant * build_game_variant_oddball(game_variant *result);
extern game_variant * build_game_variant_team_oddball(game_variant *result);
extern game_variant * build_game_variant_reverse_tag(game_variant *result);
extern game_variant * build_game_variant_accumulation(game_variant *result);
extern game_variant * build_game_variant_juggernaut(game_variant *result);
extern game_variant * build_game_variant_stalker(game_variant *result);
extern game_variant * build_game_variant_king(game_variant *result);
extern game_variant * build_game_variant_king_pro(game_variant *result);
extern game_variant * build_game_variant_crazy_king(game_variant *result);
extern game_variant * build_game_variant_team_king(game_variant *result);
extern game_variant * build_game_variant_ctf(game_variant *result);
extern game_variant * build_game_variant_ctf_pro(game_variant *result);
extern game_variant * build_game_variant_invasion(game_variant *result);
extern game_variant * build_game_variant_iron_ctf(game_variant *result);
extern game_variant * build_game_variant_race(game_variant *result);
extern game_variant * build_game_variant_rally(game_variant *result);
extern game_variant * build_game_variant_team_race(game_variant *result);
extern game_variant * build_game_variant_team_rally(game_variant *result);
extern game_variant * build_game_variant_team_slayer_standard(game_variant *result);
extern game_variant * build_game_variant_team_race_standard(game_variant *result);
extern game_variant * build_game_variant_team_oddball_standard(game_variant *result);
extern game_variant * build_game_variant_team_king_standard(game_variant *result);
extern game_variant * build_game_variant_slayer_standard(game_variant *result);
extern game_variant * build_game_variant_race_standard(game_variant *result);
extern game_variant * build_game_variant_oddball_standard(game_variant *result);
extern game_variant * build_game_variant_king_standard(game_variant *result);
extern game_variant * build_game_variant_juggernaut_standard(game_variant *result);
extern game_variant * build_game_variant_ctf_standard(game_variant *result);
extern game_variant * build_game_variant_crazy_king_standard(game_variant *result);
extern game_variant * build_game_variant_assault_standard(game_variant *result);

/* Boundary helpers (CRT / Blam runtime). */
extern int stricmp(const char *str1, const char *str2);
extern wchar_t *ascii_to_wide(const char *ascii, wchar_t *unicode, unsigned int unicode_length_bytes);
extern void playlist_profiles_enumerate_available_to_local_player_index(int16_t local_player_index, uint16_t *number_of_profiles, int *playlist_profile_indices);
extern uint8_t playlist_profile_get(int playlist_profile_index, game_variant *variant);
extern int ustrcasecmp(const wchar_t *string1, const wchar_t *string2);

/* name const-qualified 2026-07-30: read-only (stricmp table walk + widen), callers pass const strings (C4090) */
uint8_t game_engine_get_variant_by_name(const char *name, game_variant *variant)
{
    uint8_t found;
    game_variant *built_variant;
    unsigned int profile_index;
    uint16_t number_of_profiles;
    game_variant working_variant;
    game_variant variant_scratch;
    wchar_t wide_name[24];
    int playlist_profile_indices[108];

    found = 0;
    if ( !stricmp(name, "classic_slayer") )
    {
        if ( variant )
        {
            built_variant = build_game_variant_slayer(&variant_scratch);
            memcpy(&working_variant, built_variant, sizeof(working_variant));
            memcpy(variant, &working_variant, sizeof(game_variant));
        }
        found = 1;
    }
    if ( !found && !stricmp(name, "classic_slayer_pro") )
    {
        if ( variant )
        {
            built_variant = build_game_variant_slayer_pro(&variant_scratch);
            memcpy(&working_variant, built_variant, sizeof(working_variant));
            memcpy(variant, &working_variant, sizeof(game_variant));
        }
        found = 1;
    }
    if ( !found && !stricmp(name, "classic_elimination") )
    {
        if ( variant )
        {
            built_variant = build_game_variant_elimination(&variant_scratch);
            memcpy(&working_variant, built_variant, sizeof(working_variant));
            memcpy(variant, &working_variant, sizeof(game_variant));
        }
        found = 1;
    }
    if ( !found && !stricmp(name, "classic_phantoms") )
    {
        if ( variant )
        {
            built_variant = build_game_variant_phantoms(&variant_scratch);
            memcpy(&working_variant, built_variant, sizeof(working_variant));
            memcpy(variant, &working_variant, sizeof(game_variant));
        }
        found = 1;
    }
    if ( !found && !stricmp(name, "classic_endurance") )
    {
        if ( variant )
        {
            built_variant = build_game_variant_endurance(&variant_scratch);
            memcpy(&working_variant, built_variant, sizeof(working_variant));
            memcpy(variant, &working_variant, sizeof(game_variant));
        }
        found = 1;
    }
    if ( !found && !stricmp(name, "classic_rockets") )
    {
        if ( variant )
        {
            built_variant = build_game_variant_rockets(&variant_scratch);
            memcpy(&working_variant, built_variant, sizeof(working_variant));
            memcpy(variant, &working_variant, sizeof(game_variant));
        }
        found = 1;
    }
    if ( !found && !stricmp(name, "classic_snipers") )
    {
        if ( variant )
        {
            built_variant = build_game_variant_snipers(&variant_scratch);
            memcpy(&working_variant, built_variant, sizeof(working_variant));
            memcpy(variant, &working_variant, sizeof(game_variant));
        }
        found = 1;
    }
    if ( !found && !stricmp(name, "classic_team_slayer") )
    {
        if ( variant )
        {
            built_variant = build_game_variant_team_slayer(&variant_scratch);
            memcpy(&working_variant, built_variant, sizeof(working_variant));
            memcpy(variant, &working_variant, sizeof(game_variant));
        }
        found = 1;
    }
    if ( !found && !stricmp(name, "classic_oddball") )
    {
        if ( variant )
        {
            built_variant = build_game_variant_oddball(&variant_scratch);
            memcpy(&working_variant, built_variant, sizeof(working_variant));
            memcpy(variant, &working_variant, sizeof(game_variant));
        }
        found = 1;
    }
    if ( !found && !stricmp(name, "classic_team_oddball") )
    {
        if ( variant )
        {
            built_variant = build_game_variant_team_oddball(&variant_scratch);
            memcpy(&working_variant, built_variant, sizeof(working_variant));
            memcpy(variant, &working_variant, sizeof(game_variant));
        }
        found = 1;
    }
    if ( !found && !stricmp(name, "classic_reverse_tag") )
    {
        if ( variant )
        {
            built_variant = build_game_variant_reverse_tag(&variant_scratch);
            memcpy(&working_variant, built_variant, sizeof(working_variant));
            memcpy(variant, &working_variant, sizeof(game_variant));
        }
        found = 1;
    }
    if ( !found && !stricmp(name, "classic_accumulation") )
    {
        if ( variant )
        {
            built_variant = build_game_variant_accumulation(&variant_scratch);
            memcpy(&working_variant, built_variant, sizeof(working_variant));
            memcpy(variant, &working_variant, sizeof(game_variant));
        }
        found = 1;
    }
    if ( !found && !stricmp(name, "classic_juggernaut") )
    {
        if ( variant )
        {
            built_variant = build_game_variant_juggernaut(&variant_scratch);
            memcpy(&working_variant, built_variant, sizeof(working_variant));
            memcpy(variant, &working_variant, sizeof(game_variant));
        }
        found = 1;
    }
    if ( !found && !stricmp(name, "classic_stalker") )
    {
        if ( variant )
        {
            built_variant = build_game_variant_stalker(&variant_scratch);
            memcpy(&working_variant, built_variant, sizeof(working_variant));
            memcpy(variant, &working_variant, sizeof(game_variant));
        }
        found = 1;
    }
    if ( !found && !stricmp(name, "classic_king") )
    {
        if ( variant )
        {
            built_variant = build_game_variant_king(&variant_scratch);
            memcpy(&working_variant, built_variant, sizeof(working_variant));
            memcpy(variant, &working_variant, sizeof(game_variant));
        }
        found = 1;
    }
    if ( !found && !stricmp(name, "classic_king_pro") )
    {
        if ( variant )
        {
            built_variant = build_game_variant_king_pro(&variant_scratch);
            memcpy(&working_variant, built_variant, sizeof(working_variant));
            memcpy(variant, &working_variant, sizeof(game_variant));
        }
        found = 1;
    }
    if ( !found && !stricmp(name, "classic_crazy_king") )
    {
        if ( variant )
        {
            built_variant = build_game_variant_crazy_king(&variant_scratch);
            memcpy(&working_variant, built_variant, sizeof(working_variant));
            memcpy(variant, &working_variant, sizeof(game_variant));
        }
        found = 1;
    }
    if ( !found && !stricmp(name, "classic_team_king") )
    {
        if ( variant )
        {
            built_variant = build_game_variant_team_king(&variant_scratch);
            memcpy(&working_variant, built_variant, sizeof(working_variant));
            memcpy(variant, &working_variant, sizeof(game_variant));
        }
        found = 1;
    }
    if ( !found && !stricmp(name, "classic_ctf") )
    {
        if ( variant )
        {
            built_variant = build_game_variant_ctf(&variant_scratch);
            memcpy(&working_variant, built_variant, sizeof(working_variant));
            memcpy(variant, &working_variant, sizeof(game_variant));
        }
        found = 1;
    }
    if ( !found && !stricmp(name, "classic_ctf_pro") )
    {
        if ( variant )
        {
            built_variant = build_game_variant_ctf_pro(&variant_scratch);
            memcpy(&working_variant, built_variant, sizeof(working_variant));
            memcpy(variant, &working_variant, sizeof(game_variant));
        }
        found = 1;
    }
    if ( !found && !stricmp(name, "classic_invasion") )
    {
        if ( variant )
        {
            built_variant = build_game_variant_invasion(&variant_scratch);
            memcpy(&working_variant, built_variant, sizeof(working_variant));
            memcpy(variant, &working_variant, sizeof(game_variant));
        }
        found = 1;
    }
    if ( !found && !stricmp(name, "classic_iron_ctf") )
    {
        if ( variant )
        {
            built_variant = build_game_variant_iron_ctf(&variant_scratch);
            memcpy(&working_variant, built_variant, sizeof(working_variant));
            memcpy(variant, &working_variant, sizeof(game_variant));
        }
        found = 1;
    }
    if ( !found && !stricmp(name, "classic_race") )
    {
        if ( variant )
        {
            built_variant = build_game_variant_race(&variant_scratch);
            memcpy(&working_variant, built_variant, sizeof(working_variant));
            memcpy(variant, &working_variant, sizeof(game_variant));
        }
        found = 1;
    }
    if ( !found && !stricmp(name, "classic_rally") )
    {
        if ( variant )
        {
            built_variant = build_game_variant_rally(&variant_scratch);
            memcpy(&working_variant, built_variant, sizeof(working_variant));
            memcpy(variant, &working_variant, sizeof(game_variant));
        }
        found = 1;
    }
    if ( !found && !stricmp(name, "classic_team_race") )
    {
        if ( variant )
        {
            built_variant = build_game_variant_team_race(&variant_scratch);
            memcpy(&working_variant, built_variant, sizeof(working_variant));
            memcpy(variant, &working_variant, sizeof(game_variant));
        }
        found = 1;
    }
    if ( !found && !stricmp(name, "classic_team_rally") )
    {
        if ( variant )
        {
            built_variant = build_game_variant_team_rally(&variant_scratch);
            memcpy(&working_variant, built_variant, sizeof(working_variant));
            memcpy(variant, &working_variant, sizeof(game_variant));
        }
        found = 1;
    }
    if ( !found && !stricmp(name, "team_slayer") )
    {
        if ( variant )
        {
            built_variant = build_game_variant_team_slayer_standard(&variant_scratch);
            memcpy(&working_variant, built_variant, sizeof(working_variant));
            memcpy(variant, &working_variant, sizeof(game_variant));
        }
        found = 1;
    }
    if ( !found && !stricmp(name, "team_race") )
    {
        if ( variant )
        {
            built_variant = build_game_variant_team_race_standard(&variant_scratch);
            memcpy(&working_variant, built_variant, sizeof(working_variant));
            memcpy(variant, &working_variant, sizeof(game_variant));
        }
        found = 1;
    }
    if ( !found && !stricmp(name, "team_oddball") )
    {
        if ( variant )
        {
            built_variant = build_game_variant_team_oddball_standard(&variant_scratch);
            memcpy(&working_variant, built_variant, sizeof(working_variant));
            memcpy(variant, &working_variant, sizeof(game_variant));
        }
        found = 1;
    }
    if ( !found && !stricmp(name, "team_king") )
    {
        if ( variant )
        {
            built_variant = build_game_variant_team_king_standard(&variant_scratch);
            memcpy(&working_variant, built_variant, sizeof(working_variant));
            memcpy(variant, &working_variant, sizeof(game_variant));
        }
        found = 1;
    }
    if ( !found && !stricmp(name, "slayer") )
    {
        if ( variant )
        {
            built_variant = build_game_variant_slayer_standard(&variant_scratch);
            memcpy(&working_variant, built_variant, sizeof(working_variant));
            memcpy(variant, &working_variant, sizeof(game_variant));
        }
        found = 1;
    }
    if ( !found && !stricmp(name, "race") )
    {
        if ( variant )
        {
            built_variant = build_game_variant_race_standard(&variant_scratch);
            memcpy(&working_variant, built_variant, sizeof(working_variant));
            memcpy(variant, &working_variant, sizeof(game_variant));
        }
        found = 1;
    }
    if ( !found && !stricmp(name, "oddball") )
    {
        if ( variant )
        {
            built_variant = build_game_variant_oddball_standard(&variant_scratch);
            memcpy(&working_variant, built_variant, sizeof(working_variant));
            memcpy(variant, &working_variant, sizeof(game_variant));
        }
        found = 1;
    }
    if ( !found && !stricmp(name, "king") )
    {
        if ( variant )
        {
            built_variant = build_game_variant_king_standard(&variant_scratch);
            memcpy(&working_variant, built_variant, sizeof(working_variant));
            memcpy(variant, &working_variant, sizeof(game_variant));
        }
        found = 1;
    }
    if ( !found && !stricmp(name, "juggernaut") )
    {
        if ( variant )
        {
            built_variant = build_game_variant_juggernaut_standard(&variant_scratch);
            memcpy(&working_variant, built_variant, sizeof(working_variant));
            memcpy(variant, &working_variant, sizeof(game_variant));
        }
        found = 1;
    }
    if ( !found && !stricmp(name, "ctf") )
    {
        if ( variant )
        {
            built_variant = build_game_variant_ctf_standard(&variant_scratch);
            memcpy(&working_variant, built_variant, sizeof(working_variant));
            memcpy(variant, &working_variant, sizeof(game_variant));
        }
        found = 1;
    }
    if ( !found && !stricmp(name, "crazy_king") )
    {
        if ( variant )
        {
            built_variant = build_game_variant_crazy_king_standard(&variant_scratch);
            memcpy(&working_variant, built_variant, sizeof(working_variant));
            memcpy(variant, &working_variant, sizeof(game_variant));
        }
        found = 1;
    }
    if ( !found && !stricmp(name, "assault") )
    {
        if ( variant )
        {
            built_variant = build_game_variant_assault_standard(&variant_scratch);
            memcpy(&working_variant, built_variant, sizeof(working_variant));
            memcpy(variant, &working_variant, sizeof(game_variant));
        }
        found = 1;
    }
    if ( found )
        return found;

    /* Not a built-in preset — search the local player's available playlist profiles by name. */
    number_of_profiles = 100; /* in: buffer capacity; out: count enumerated */
    ascii_to_wide(name, wide_name, 0x30u);
    playlist_profiles_enumerate_available_to_local_player_index(0, &number_of_profiles, playlist_profile_indices);
    if ( !number_of_profiles )
        return found;

    profile_index = 0;
    while ( !playlist_profile_get(playlist_profile_indices[profile_index], &working_variant)
            || ustrcasecmp(working_variant.human_readable_game_description, wide_name) )
    {
        profile_index = (uint16_t)(profile_index + 1);
        if ( profile_index >= number_of_profiles )
            return found;
    }

    if ( variant )
        memcpy(variant, &working_variant, sizeof(game_variant));
    return 1;
}
