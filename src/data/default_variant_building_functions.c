/* default_variant_building_functions @ 0x841815F8 (.data, 152 bytes)
 * DB applied_types: game_variant *(__fastcall *default_variant_building_functions[38])(game_variant *result);
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x8374E898 -> build_game_variant_slayer
 *   +0x0004 = 0x8374E970 -> build_game_variant_slayer_pro
 *   +0x0008 = 0x8374EA48 -> build_game_variant_elimination
 *   +0x000C = 0x8374EB20 -> build_game_variant_phantoms
 *   +0x0010 = 0x8374EBF8 -> build_game_variant_endurance
 *   +0x0014 = 0x8374ECD8 -> build_game_variant_rockets
 *   +0x0018 = 0x8374EDB0 -> build_game_variant_snipers
 *   +0x001C = 0x8374EF68 -> build_game_variant_oddball
 *   +0x0020 = 0x8374F100 -> build_game_variant_reverse_tag
 *   +0x0024 = 0x8374F1E8 -> build_game_variant_accumulation
 *   +0x0028 = 0x8374F2D8 -> build_game_variant_juggernaut
 *   +0x002C = 0x8374F3C0 -> build_game_variant_stalker
 *   +0x0030 = 0x8374F4B0 -> build_game_variant_king
 *   +0x0034 = 0x8374F580 -> build_game_variant_king_pro
 *   +0x0038 = 0x8374F658 -> build_game_variant_crazy_king
 *   +0x003C = 0x8374FB80 -> build_game_variant_race
 *   +0x0040 = 0x8374FC58 -> build_game_variant_rally
 *   +0x0044 = 0x8374F800 -> build_game_variant_ctf
 *   +0x0048 = 0x8374F9C0 -> build_game_variant_invasion
 *   +0x004C = 0x8374FAA0 -> build_game_variant_iron_ctf
 *   +0x0050 = 0x8374F8E0 -> build_game_variant_ctf_pro
 *   +0x0054 = 0x8374FD30 -> build_game_variant_team_race
 *   +0x0058 = 0x8374FE08 -> build_game_variant_team_rally
 *   +0x005C = 0x8374F050 -> build_game_variant_team_oddball
 *   +0x0060 = 0x8374F728 -> build_game_variant_team_king
 *   +0x0064 = 0x8374EE90 -> build_game_variant_team_slayer
 *   +0x0068 = 0x8374AF90 -> build_game_variant_slayer_standard
 *   +0x006C = 0x8374ADC0 -> build_game_variant_oddball_standard
 *   +0x0070 = 0x8374ABF8 -> build_game_variant_juggernaut_standard
 *   +0x0074 = 0x8374ACE8 -> build_game_variant_king_standard
 *   +0x0078 = 0x8374AA38 -> build_game_variant_crazy_king_standard
 *   +0x007C = 0x8374AEB0 -> build_game_variant_race_standard
 *   +0x0080 = 0x8374AB10 -> build_game_variant_ctf_standard
 *   +0x0084 = 0x8374A950 -> build_game_variant_assault_standard
 *   +0x0088 = 0x8374B320 -> build_game_variant_team_slayer_standard
 *   +0x008C = 0x8374B150 -> build_game_variant_team_oddball_standard
 *   +0x0090 = 0x8374B070 -> build_game_variant_team_king_standard
 *   +0x0094 = 0x8374B240 -> build_game_variant_team_race_standard
 * Blam .lib data-global storage definition (init batch 7).
 */
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
