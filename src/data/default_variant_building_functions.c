/* Blam .lib data-global storage definition (init batch 7). */
#include "../headers/game_variant.h"
/* default_variant_building_functions @0x841815F8, DB extent 152 = 38 builder fn pointers. */
extern game_variant * build_game_variant_slayer(game_variant *result);
extern game_variant * build_game_variant_slayer_pro(game_variant *result);
extern game_variant * build_game_variant_elimination(game_variant *result);
extern game_variant * build_game_variant_phantoms(game_variant *result);
extern game_variant * build_game_variant_endurance(game_variant *result);
extern game_variant * build_game_variant_rockets(game_variant *result);
extern game_variant * build_game_variant_snipers(game_variant *result);
extern game_variant * build_game_variant_oddball(game_variant *result);
extern game_variant * build_game_variant_reverse_tag(game_variant *result);
extern game_variant * build_game_variant_accumulation(game_variant *result);
extern game_variant * build_game_variant_juggernaut(game_variant *result);
extern game_variant * build_game_variant_stalker(game_variant *result);
extern game_variant * build_game_variant_king(game_variant *result);
extern game_variant * build_game_variant_king_pro(game_variant *result);
extern game_variant * build_game_variant_crazy_king(game_variant *result);
extern game_variant * build_game_variant_race(game_variant *result);
extern game_variant * build_game_variant_rally(game_variant *result);
extern game_variant * build_game_variant_ctf(game_variant *result);
extern game_variant * build_game_variant_invasion(game_variant *result);
extern game_variant * build_game_variant_iron_ctf(game_variant *result);
extern game_variant * build_game_variant_ctf_pro(game_variant *result);
extern game_variant * build_game_variant_team_race(game_variant *result);
extern game_variant * build_game_variant_team_rally(game_variant *result);
extern game_variant * build_game_variant_team_oddball(game_variant *result);
extern game_variant * build_game_variant_team_king(game_variant *result);
extern game_variant * build_game_variant_team_slayer(game_variant *result);
extern game_variant * build_game_variant_slayer_standard(game_variant *result);
extern game_variant * build_game_variant_oddball_standard(game_variant *result);
extern game_variant * build_game_variant_juggernaut_standard(game_variant *result);
extern game_variant * build_game_variant_king_standard(game_variant *result);
extern game_variant * build_game_variant_crazy_king_standard(game_variant *result);
extern game_variant * build_game_variant_race_standard(game_variant *result);
extern game_variant * build_game_variant_ctf_standard(game_variant *result);
extern game_variant * build_game_variant_assault_standard(game_variant *result);
extern game_variant * build_game_variant_team_slayer_standard(game_variant *result);
extern game_variant * build_game_variant_team_oddball_standard(game_variant *result);
extern game_variant * build_game_variant_team_king_standard(game_variant *result);
extern game_variant * build_game_variant_team_race_standard(game_variant *result);

game_variant *(*default_variant_building_functions[])(game_variant *) = {
    build_game_variant_slayer,
    build_game_variant_slayer_pro,
    build_game_variant_elimination,
    build_game_variant_phantoms,
    build_game_variant_endurance,
    build_game_variant_rockets,
    build_game_variant_snipers,
    build_game_variant_oddball,
    build_game_variant_reverse_tag,
    build_game_variant_accumulation,
    build_game_variant_juggernaut,
    build_game_variant_stalker,
    build_game_variant_king,
    build_game_variant_king_pro,
    build_game_variant_crazy_king,
    build_game_variant_race,
    build_game_variant_rally,
    build_game_variant_ctf,
    build_game_variant_invasion,
    build_game_variant_iron_ctf,
    build_game_variant_ctf_pro,
    build_game_variant_team_race,
    build_game_variant_team_rally,
    build_game_variant_team_oddball,
    build_game_variant_team_king,
    build_game_variant_team_slayer,
    build_game_variant_slayer_standard,
    build_game_variant_oddball_standard,
    build_game_variant_juggernaut_standard,
    build_game_variant_king_standard,
    build_game_variant_crazy_king_standard,
    build_game_variant_race_standard,
    build_game_variant_ctf_standard,
    build_game_variant_assault_standard,
    build_game_variant_team_slayer_standard,
    build_game_variant_team_oddball_standard,
    build_game_variant_team_king_standard,
    build_game_variant_team_race_standard,
};
