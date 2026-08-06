/* dialogue_vocalization_lookup @ 0x82129AD8 (.rdata, 418 bytes)
 * DB applied_types: const __int16 dialogue_vocalization_lookup[209];
 * Image bytes (big-endian), decoded from the binary .rdata record:
 *   +0x0000: 0001 FFFF FFFF FFFF FFFF FFFF FFFF 0006
 *   +0x0010: 0006 0006 FFFF 000A FFFF 000C 000F FFFF
 *   +0x0020: 000E 000F 000E 000F FFFF FFFF 0015 FFFF
 *   +0x0030: FFFF FFFF FFFF FFFF FFFF FFFF FFFF 001D
 *   +0x0040: FFFF FFFF FFFF 0020 0020 0020 0020 0020
 *   +0x0050: 0020 0020 0020 0020 0020 0020 FFFF FFFF
 *   +0x0060: FFFF FFFF FFFF 0031 FFFF FFFF FFFF 0035
 *   +0x0070: 0036 0035 0036 0035 0036 0035 0036 0035
 *   +0x0080: 0036 0035 0035 0035 0035 0035 0035 0035
 *   +0x0090: 0035 0035 0035 0035 FFFF FFFF FFFF FFFF
 *   +0x00A0: FFFF 0050 0050 0050 0050 0050 0050 0050
 *   +0x00B0: 0050 0050 0050 0050 0050 005D 005E 005F
 *   +0x00C0: FFFF 0060 0060 0062 0060 0064 0064 0064
 *   +0x00D0: 0064 FFFF FFFF FFFF 006C 006D 006C FFFF
 *   +0x00E0: FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF
 *   +0x00F0: FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF
 *   +0x0100: FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF
 *   +0x0110: FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF
 *   +0x0120: FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF
 *   +0x0130: FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF
 *   +0x0140: FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF
 *   +0x0150: FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF
 *   +0x0160: FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF
 *   +0x0170: FFFF FFFF FFFF FFFF 0035 0070 009A 00BF
 *   +0x0180: FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF
 *   +0x0190: FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF
 *   +0x01A0: 0000
 * CAVEAT: six entries name themselves - [93]/[94]/[95] (playerkill_unused1..3), [108]
 * (sighted_enemy_new), [109] (sighted_enemy_recent) and [191] (shoot_corpse). unit_test_speech's
 * fallback walk re-indexes with the value it just read, so such an entry cannot terminate the walk
 * on its own; it only exits because the resolved sound is non-null or recursion is disabled. The
 * bytes were re-read straight out of the image and are reproduced exactly - this is original
 * shipped data, not a reconstruction artifact.
 * .rdata read-only table, 418 bytes of data.
 * ai_vocalization_type -> the fallback ai_vocalization_type to try when a unit's dialogue tag has
 * no sound for the requested one, or -1 to give up. unit_test_speech.c:52 is the only consumer:
 * `vocalization_type = dialogue_vocalization_lookup[type];` inside the recursive-lookup loop.
 * Element type int16_t, proven by the load in unit_test_speech (index scaled by 2, halfword load,
 * sign-extended):
 *   837BA73C  lis   r10, dialogue_vocalization_lookup@ha
 *   837BA740  addi  r7, r10, dialogue_vocalization_lookup@l
 *   837BA764  slwi  r11, r11, 1
 *   837BA768  lhzx  r4, r11, r7
 *   837BA76C  extsh r11, r4
 * Length 209 == NUMBER_OF_VOCALIZATION_TYPES, which is the same domain that sizes
 * dialogue_definition.vocalizations[209] - the table this one redirects within. 209 * 2 == 418;
 * the gap to the next named .rdata object (aFrontLeafIndex @0x82129C7C) is 420, so the trailing
 * two bytes are alignment padding, not an element.
 * Entry [208] (postcombatchatter_unused4) is 0x0000. Written here as the enum constant that value
 * denotes; note it is equally consistent with a 208-entry initializer list that the compiler
 * zero-filled - both spellings assemble to the identical image, and [205]..[207] are -1.
 * 132 of the 209 entries are -1 (no fallback).
 * /
 */
#include <stdint.h>
#include "../headers/ai_vocalization_type.h"

int16_t dialogue_vocalization_lookup[NUMBER_OF_VOCALIZATION_TYPES] =
{
    _vocalization_idle_combat,                        /* [  0] _vocalization_idle_noncombat */
    -1,                                               /* [  1] _vocalization_idle_combat */
    -1,                                               /* [  2] _vocalization_idle_flee */
    -1,                                               /* [  3] _vocalization_idle_unused2 */
    -1,                                               /* [  4] _vocalization_idle_unused3 */
    -1,                                               /* [  5] _vocalization_idle_unused4 */
    -1,                                               /* [  6] _vocalization_pain_body */
    _vocalization_pain_body,                          /* [  7] _vocalization_pain_body_major */
    _vocalization_pain_body,                          /* [  8] _vocalization_pain_shield */
    _vocalization_pain_body,                          /* [  9] _vocalization_pain_falling */
    -1,                                               /* [ 10] _vocalization_scream_fear */
    _vocalization_scream_fear,                        /* [ 11] _vocalization_scream_pain */
    -1,                                               /* [ 12] _vocalization_maimed_limb */
    _vocalization_maimed_limb,                        /* [ 13] _vocalization_maimed_head */
    _vocalization_death_violent,                      /* [ 14] _vocalization_death_quiet */
    -1,                                               /* [ 15] _vocalization_death_violent */
    _vocalization_death_quiet,                        /* [ 16] _vocalization_death_falling */
    _vocalization_death_violent,                      /* [ 17] _vocalization_death_agonizing */
    _vocalization_death_quiet,                        /* [ 18] _vocalization_death_instant */
    _vocalization_death_violent,                      /* [ 19] _vocalization_death_flying */
    -1,                                               /* [ 20] _vocalization_involuntary_unused1 */
    -1,                                               /* [ 21] _vocalization_shot_friend */
    _vocalization_shot_friend,                        /* [ 22] _vocalization_shot_friend_player */
    -1,                                               /* [ 23] _vocalization_shot_enemy */
    -1,                                               /* [ 24] _vocalization_shot_enemy_comment */
    -1,                                               /* [ 25] _vocalization_hurtingpeople_unused1 */
    -1,                                               /* [ 26] _vocalization_hurtingpeople_unused2 */
    -1,                                               /* [ 27] _vocalization_hurtingpeople_unused3 */
    -1,                                               /* [ 28] _vocalization_hurtingpeople_unused4 */
    -1,                                               /* [ 29] _vocalization_hurt_friend */
    -1,                                               /* [ 30] _vocalization_hurt_friend_reply */
    _vocalization_hurt_friend,                        /* [ 31] _vocalization_hurt_friend_player */
    -1,                                               /* [ 32] _vocalization_hurt_enemy */
    -1,                                               /* [ 33] _vocalization_hurt_enemy_reply */
    -1,                                               /* [ 34] _vocalization_hurt_enemy_comment */
    _vocalization_hurt_enemy,                         /* [ 35] _vocalization_hurt_enemy_bullet */
    _vocalization_hurt_enemy,                         /* [ 36] _vocalization_hurt_enemy_needler */
    _vocalization_hurt_enemy,                         /* [ 37] _vocalization_hurt_enemy_plasma */
    _vocalization_hurt_enemy,                         /* [ 38] _vocalization_hurt_enemy_sniper */
    _vocalization_hurt_enemy,                         /* [ 39] _vocalization_hurt_enemy_grenade */
    _vocalization_hurt_enemy,                         /* [ 40] _vocalization_hurt_enemy_explosion */
    _vocalization_hurt_enemy,                         /* [ 41] _vocalization_hurt_enemy_melee */
    _vocalization_hurt_enemy,                         /* [ 42] _vocalization_hurt_enemy_flame */
    _vocalization_hurt_enemy,                         /* [ 43] _vocalization_hurt_enemy_shotgun */
    _vocalization_hurt_enemy,                         /* [ 44] _vocalization_hurt_enemy_vehicle */
    _vocalization_hurt_enemy,                         /* [ 45] _vocalization_hurt_enemy_mountedweapon */
    -1,                                               /* [ 46] _vocalization_hurt_unused1 */
    -1,                                               /* [ 47] _vocalization_hurt_unused2 */
    -1,                                               /* [ 48] _vocalization_hurt_unused3 */
    -1,                                               /* [ 49] _vocalization_killed_friend */
    -1,                                               /* [ 50] _vocalization_killed_friend_comment */
    _vocalization_killed_friend,                      /* [ 51] _vocalization_killed_friend_player */
    -1,                                               /* [ 52] _vocalization_killed_friend_player_comment */
    -1,                                               /* [ 53] _vocalization_killed_enemy */
    -1,                                               /* [ 54] _vocalization_killed_enemy_comment */
    _vocalization_killed_enemy,                       /* [ 55] _vocalization_killed_enemy_player */
    _vocalization_killed_enemy_comment,               /* [ 56] _vocalization_killed_enemy_player_comment */
    _vocalization_killed_enemy,                       /* [ 57] _vocalization_killed_enemy_covenant */
    _vocalization_killed_enemy_comment,               /* [ 58] _vocalization_killed_enemy_covenant_comment */
    _vocalization_killed_enemy,                       /* [ 59] _vocalization_killed_enemy_floodcombat */
    _vocalization_killed_enemy_comment,               /* [ 60] _vocalization_killed_enemy_floodcombat_comment */
    _vocalization_killed_enemy,                       /* [ 61] _vocalization_killed_enemy_floodcarrier */
    _vocalization_killed_enemy_comment,               /* [ 62] _vocalization_killed_enemy_floodcarrier_comment */
    _vocalization_killed_enemy,                       /* [ 63] _vocalization_killed_enemy_sentinel */
    _vocalization_killed_enemy_comment,               /* [ 64] _vocalization_killed_enemy_sentinel_comment */
    _vocalization_killed_enemy,                       /* [ 65] _vocalization_killed_enemy_bullet */
    _vocalization_killed_enemy,                       /* [ 66] _vocalization_killed_enemy_needler */
    _vocalization_killed_enemy,                       /* [ 67] _vocalization_killed_enemy_plasma */
    _vocalization_killed_enemy,                       /* [ 68] _vocalization_killed_enemy_sniper */
    _vocalization_killed_enemy,                       /* [ 69] _vocalization_killed_enemy_grenade */
    _vocalization_killed_enemy,                       /* [ 70] _vocalization_killed_enemy_explosion */
    _vocalization_killed_enemy,                       /* [ 71] _vocalization_killed_enemy_melee */
    _vocalization_killed_enemy,                       /* [ 72] _vocalization_killed_enemy_flame */
    _vocalization_killed_enemy,                       /* [ 73] _vocalization_killed_enemy_shotgun */
    _vocalization_killed_enemy,                       /* [ 74] _vocalization_killed_enemy_vehicle */
    _vocalization_killed_enemy,                       /* [ 75] _vocalization_killed_enemy_mountedweapon */
    -1,                                               /* [ 76] _vocalization_killing_spree */
    -1,                                               /* [ 77] _vocalization_killingpeople_unused1 */
    -1,                                               /* [ 78] _vocalization_killingpeople_unused2 */
    -1,                                               /* [ 79] _vocalization_killingpeople_unused3 */
    -1,                                               /* [ 80] _vocalization_player_kill_comment */
    _vocalization_player_kill_comment,                /* [ 81] _vocalization_player_kill_bullet_comment */
    _vocalization_player_kill_comment,                /* [ 82] _vocalization_player_kill_needler_comment */
    _vocalization_player_kill_comment,                /* [ 83] _vocalization_player_kill_plasma_comment */
    _vocalization_player_kill_comment,                /* [ 84] _vocalization_player_kill_sniper_comment */
    _vocalization_player_kill_comment,                /* [ 85] _vocalization_player_kill_grenade_comment */
    _vocalization_player_kill_comment,                /* [ 86] _vocalization_player_kill_explosion_comment */
    _vocalization_player_kill_comment,                /* [ 87] _vocalization_player_kill_melee_comment */
    _vocalization_player_kill_comment,                /* [ 88] _vocalization_player_kill_flame_comment */
    _vocalization_player_kill_comment,                /* [ 89] _vocalization_player_kill_shotgun_comment */
    _vocalization_player_kill_comment,                /* [ 90] _vocalization_player_kill_vehicle_comment */
    _vocalization_player_kill_comment,                /* [ 91] _vocalization_player_kill_mountedweapon_comment */
    _vocalization_player_kill_comment,                /* [ 92] _vocalization_player_killing_spree_comment */
    _vocalization_playerkill_unused1,                 /* [ 93] _vocalization_playerkill_unused1 */
    _vocalization_playerkill_unused2,                 /* [ 94] _vocalization_playerkill_unused2 */
    _vocalization_playerkill_unused3,                 /* [ 95] _vocalization_playerkill_unused3 */
    -1,                                               /* [ 96] _vocalization_friend_died */
    _vocalization_friend_died,                        /* [ 97] _vocalization_friend_player_died */
    _vocalization_friend_died,                        /* [ 98] _vocalization_friend_killed_by_friend */
    _vocalization_friend_killed_by_friend,            /* [ 99] _vocalization_friend_killed_by_friend_player */
    _vocalization_friend_died,                        /* [100] _vocalization_friend_killed_by_enemy */
    _vocalization_friend_killed_by_enemy,             /* [101] _vocalization_friend_killed_by_enemy_player */
    _vocalization_friend_killed_by_enemy,             /* [102] _vocalization_friend_killed_by_covenant */
    _vocalization_friend_killed_by_enemy,             /* [103] _vocalization_friend_killed_by_flood */
    _vocalization_friend_killed_by_enemy,             /* [104] _vocalization_friend_killed_by_sentinel */
    -1,                                               /* [105] _vocalization_friend_betrayed */
    -1,                                               /* [106] _vocalization_friendsdying_unused1 */
    -1,                                               /* [107] _vocalization_friendsdying_unused2 */
    _vocalization_sighted_enemy_new,                  /* [108] _vocalization_sighted_enemy_new */
    _vocalization_sighted_enemy_recent,               /* [109] _vocalization_sighted_enemy_recent */
    _vocalization_sighted_enemy_new,                  /* [110] _vocalization_sighted_enemy_searching */
    -1,                                               /* [111] _vocalization_unexpected_enemy */
    -1,                                               /* [112] _vocalization_dead_friend_found */
    -1,                                               /* [113] _vocalization_allegiance_broken */
    -1,                                               /* [114] _vocalization_allegiance_reformed */
    -1,                                               /* [115] _vocalization_grenade_throwing */
    -1,                                               /* [116] _vocalization_grenade_startle */
    -1,                                               /* [117] _vocalization_grenade_sighted */
    -1,                                               /* [118] _vocalization_grenade_danger_enemy */
    -1,                                               /* [119] _vocalization_grenade_danger_self */
    -1,                                               /* [120] _vocalization_grenade_danger_friend */
    -1,                                               /* [121] _vocalization_shouting_unused1 */
    -1,                                               /* [122] _vocalization_shouting_unused2 */
    -1,                                               /* [123] _vocalization_sighted_enemy_near_reply */
    -1,                                               /* [124] _vocalization_sighted_enemy_far_reply */
    -1,                                               /* [125] _vocalization_alert_noncombat */
    -1,                                               /* [126] _vocalization_alert_noncombat_reply */
    -1,                                               /* [127] _vocalization_alert_lost_contact */
    -1,                                               /* [128] _vocalization_alert_lost_contact_reply */
    -1,                                               /* [129] _vocalization_blocked */
    -1,                                               /* [130] _vocalization_blocked_reply */
    -1,                                               /* [131] _vocalization_search_start */
    -1,                                               /* [132] _vocalization_search_query */
    -1,                                               /* [133] _vocalization_search_query_reply */
    -1,                                               /* [134] _vocalization_search_report */
    -1,                                               /* [135] _vocalization_search_abandon */
    -1,                                               /* [136] _vocalization_search_group_abandon */
    -1,                                               /* [137] _vocalization_uncover_start */
    -1,                                               /* [138] _vocalization_uncover_start_reply */
    -1,                                               /* [139] _vocalization_advance */
    -1,                                               /* [140] _vocalization_advance_reply */
    -1,                                               /* [141] _vocalization_retreat */
    -1,                                               /* [142] _vocalization_retreat_reply */
    -1,                                               /* [143] _vocalization_cover */
    -1,                                               /* [144] _vocalization_group_unused1 */
    -1,                                               /* [145] _vocalization_group_unused2 */
    -1,                                               /* [146] _vocalization_group_unused3 */
    -1,                                               /* [147] _vocalization_group_unused4 */
    -1,                                               /* [148] _vocalization_sighted_friend_player */
    -1,                                               /* [149] _vocalization_shooting */
    -1,                                               /* [150] _vocalization_shooting_vehicle */
    -1,                                               /* [151] _vocalization_shooting_berserk */
    -1,                                               /* [152] _vocalization_shooting_group */
    -1,                                               /* [153] _vocalization_shooting_traitor */
    -1,                                               /* [154] _vocalization_taunt */
    -1,                                               /* [155] _vocalization_taunt_reply */
    -1,                                               /* [156] _vocalization_flee */
    -1,                                               /* [157] _vocalization_flee_reply */
    -1,                                               /* [158] _vocalization_flee_leader_died */
    -1,                                               /* [159] _vocalization_attempted_flee */
    -1,                                               /* [160] _vocalization_attempted_flee_reply */
    -1,                                               /* [161] _vocalization_lost_contact */
    -1,                                               /* [162] _vocalization_hiding_finished */
    -1,                                               /* [163] _vocalization_vehicle_entry */
    -1,                                               /* [164] _vocalization_vehicle_exit */
    -1,                                               /* [165] _vocalization_vehicle_woohoo */
    -1,                                               /* [166] _vocalization_vehicle_scared */
    -1,                                               /* [167] _vocalization_vehicle_collision */
    -1,                                               /* [168] _vocalization_partially_sighted */
    -1,                                               /* [169] _vocalization_nothing_there */
    -1,                                               /* [170] _vocalization_pleading */
    -1,                                               /* [171] _vocalization_actions_unused1 */
    -1,                                               /* [172] _vocalization_actions_unused2 */
    -1,                                               /* [173] _vocalization_actions_unused3 */
    -1,                                               /* [174] _vocalization_actions_unused4 */
    -1,                                               /* [175] _vocalization_actions_unused5 */
    -1,                                               /* [176] _vocalization_actions_unused6 */
    -1,                                               /* [177] _vocalization_surprise */
    -1,                                               /* [178] _vocalization_berserk */
    -1,                                               /* [179] _vocalization_melee */
    -1,                                               /* [180] _vocalization_dive */
    -1,                                               /* [181] _vocalization_uncover_exclamation */
    -1,                                               /* [182] _vocalization_leap */
    -1,                                               /* [183] _vocalization_resurrect */
    -1,                                               /* [184] _vocalization_exclamation_unused1 */
    -1,                                               /* [185] _vocalization_exclamation_unused2 */
    -1,                                               /* [186] _vocalization_exclamation_unused3 */
    -1,                                               /* [187] _vocalization_exclamation_unused4 */
    _vocalization_killed_enemy,                       /* [188] _vocalization_celebrate */
    _vocalization_dead_friend_found,                  /* [189] _vocalization_check_body_enemy */
    _vocalization_taunt,                              /* [190] _vocalization_check_body_friend */
    _vocalization_shoot_corpse,                       /* [191] _vocalization_shoot_corpse */
    -1,                                               /* [192] _vocalization_shoot_corpse_player */
    -1,                                               /* [193] _vocalization_postcombataction_unused1 */
    -1,                                               /* [194] _vocalization_postcombataction_unused2 */
    -1,                                               /* [195] _vocalization_postcombataction_unused3 */
    -1,                                               /* [196] _vocalization_postcombataction_unused4 */
    -1,                                               /* [197] _vocalization_postcombat_alone */
    -1,                                               /* [198] _vocalization_postcombat_unscathed */
    -1,                                               /* [199] _vocalization_postcombat_wounded */
    -1,                                               /* [200] _vocalization_postcombat_wounded_reply */
    -1,                                               /* [201] _vocalization_postcombat_massacre */
    -1,                                               /* [202] _vocalization_postcombat_massacre_reply */
    -1,                                               /* [203] _vocalization_postcombat_triumph */
    -1,                                               /* [204] _vocalization_postcombat_triumph_reply */
    -1,                                               /* [205] _vocalization_postcombatchatter_unused1 */
    -1,                                               /* [206] _vocalization_postcombatchatter_unused2 */
    -1,                                               /* [207] _vocalization_postcombatchatter_unused3 */
    _vocalization_idle_noncombat,                     /* [208] _vocalization_postcombatchatter_unused4 */
};
