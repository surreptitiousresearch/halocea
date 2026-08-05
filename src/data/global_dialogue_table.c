/* global_dialogue_table @ 0x821269A0 (.rdata, 4200 bytes) — the master AI dialogue table: every
 * (communication_type, matching conditions) -> vocalization/animation row ai_communication_event
 * may choose from. Rows are grouped by communication_type in ascending order, which is what lets
 * ai_communication_initialize build global_communication_table_indices by scanning for the first
 * row of each type, and what lets ai_communication_event walk the contiguous run for one type.
 *
 * Element type and length are PROVEN, not inferred from the (contradictory) corpus declarations:
 * ai_communication_initialize @0x837CBA18 forms the object address directly (`lis r11,
 * global_dialogue_table@ha` / `addi r30, r11, global_dialogue_table@l` — no pointer load) and
 * strides it with `lhzu r9, 0x28(r10)`; ai_communication_update_speech_timers computes
 * `40 * index` and loads +0x14 (repeat_delay). 0x28 == 40 == sizeof(dialogue_usage) and
 * 4200 / 40 == 105 rows exactly. Row [104] is the terminator (communication_type == -1); the
 * initialize loop counts up to it, yielding global_dialogue_event_count == 104.
 * Every row's bytes +0x26..+0x27 are zero — the struct's tail alignment padding.
 *
 * Field order per row (dialogue_usage, DB-verified layout):
 *   communication_type, communication_priority, vocalization_type, animation_type,
 *   protagonist_type, protagonist_look_priority, recipient_look_direction, recipient_look_priority,
 *   weight, repeat_delay, flags,
 *   required_group, required_hostility, required_enemy_status,
 *   required_subject_race, required_cause_race, required_damage
 * -1 in any int16_t field is the "no requirement / none" sentinel.
 */
#include "../headers/dialogue_usage.h"
#include "../headers/ai_communication_type.h"
#include "../headers/ai_communication_priority.h"
#include "../headers/ai_communication_protagonist_type.h"
#include "../headers/ai_communication_look_direction.h"
#include "../headers/ai_communication_hostility.h"
#include "../headers/ai_communication_enemy_status.h"
#include "../headers/ai_communication_group.h"
#include "../headers/ai_vocalization_type.h"
#include "../headers/unit_animation_impulse.h"
#include "../headers/dialogue_usage_flags.h"
#include "../headers/damage_category.h"
#include "../headers/actor_race.h"

const dialogue_usage global_dialogue_table[105] =
{
    /* [  0] _ai_communication_death */
    { _ai_communication_death, _ai_communication_priority_talk, _vocalization_killed_friend, -1,
      _comm_protagonist_cause, _ai_communication_priority_yell, _comm_look_direction_protagonist, _ai_communication_priority_filler,
      10.0f, 0.0f, 0, -1, _comm_hostility_friend, -1, -1, -1, -1 },
    /* [  1] _ai_communication_death */
    { _ai_communication_death, _ai_communication_priority_yell, _vocalization_killed_friend_player, -1,
      _comm_protagonist_cause, _ai_communication_priority_yell, _comm_look_direction_target, _ai_communication_priority_yell,
      20.0f, 0.0f, (1 << _dialogue_usage_force_bit) | (1 << _dialogue_usage_override_scripted_bit), -1, _comm_hostility_friend, -1, _race_player, -1, -1 },
    /* [  2] _ai_communication_death */
    { _ai_communication_death, _ai_communication_priority_chatter, _vocalization_killed_enemy, -1,
      _comm_protagonist_cause, _ai_communication_priority_filler, _comm_look_direction_protagonist, _ai_communication_priority_filler,
      10.0f, 0.0f, (1 << _dialogue_usage_player_bit), -1, _comm_hostility_enemy, -1, -1, -1, -1 },
    /* [  3] _ai_communication_death */
    { _ai_communication_death, _ai_communication_priority_yell, _vocalization_killed_enemy_player, -1,
      _comm_protagonist_cause, _ai_communication_priority_yell, _comm_look_direction_target, _ai_communication_priority_yell,
      20.0f, 0.0f, (1 << _dialogue_usage_force_bit) | (1 << _dialogue_usage_player_bit) | (1 << _dialogue_usage_override_scripted_bit), -1, _comm_hostility_enemy, -1, _race_player, -1, -1 },
    /* [  4] _ai_communication_death */
    { _ai_communication_death, _ai_communication_priority_chatter, _vocalization_killed_enemy_covenant, -1,
      _comm_protagonist_cause, _ai_communication_priority_filler, _comm_look_direction_protagonist, _ai_communication_priority_filler,
      10.0f, 0.0f, (1 << _dialogue_usage_player_bit), -1, _comm_hostility_enemy, -1, _race_covenant, -1, -1 },
    /* [  5] _ai_communication_death */
    { _ai_communication_death, _ai_communication_priority_chatter, _vocalization_killed_enemy_floodcombat, -1,
      _comm_protagonist_cause, _ai_communication_priority_filler, _comm_look_direction_protagonist, _ai_communication_priority_filler,
      40.0f, 0.0f, (1 << _dialogue_usage_player_bit), -1, _comm_hostility_enemy, -1, _race_floodcombat, -1, -1 },
    /* [  6] _ai_communication_death */
    { _ai_communication_death, _ai_communication_priority_chatter, _vocalization_killed_enemy_floodcarrier, -1,
      _comm_protagonist_cause, _ai_communication_priority_filler, _comm_look_direction_protagonist, _ai_communication_priority_filler,
      40.0f, 0.0f, (1 << _dialogue_usage_player_bit), -1, _comm_hostility_enemy, -1, _race_floodcarrier, -1, -1 },
    /* [  7] _ai_communication_death */
    { _ai_communication_death, _ai_communication_priority_chatter, _vocalization_killed_enemy_sentinel, -1,
      _comm_protagonist_cause, _ai_communication_priority_filler, _comm_look_direction_protagonist, _ai_communication_priority_filler,
      40.0f, 0.0f, (1 << _dialogue_usage_player_bit), -1, _comm_hostility_enemy, -1, _race_sentinel, -1, -1 },
    /* [  8] _ai_communication_death */
    { _ai_communication_death, _ai_communication_priority_chatter, _vocalization_killed_enemy_bullet, -1,
      _comm_protagonist_cause, _ai_communication_priority_filler, _comm_look_direction_protagonist, _ai_communication_priority_filler,
      10.0f, 10.0f, 0, -1, _comm_hostility_enemy, -1, -1, -1, _damage_category_bullet },
    /* [  9] _ai_communication_death */
    { _ai_communication_death, _ai_communication_priority_chatter, _vocalization_killed_enemy_plasma, -1,
      _comm_protagonist_cause, _ai_communication_priority_filler, _comm_look_direction_protagonist, _ai_communication_priority_filler,
      10.0f, 10.0f, 0, -1, _comm_hostility_enemy, -1, -1, -1, _damage_category_plasma },
    /* [ 10] _ai_communication_death */
    { _ai_communication_death, _ai_communication_priority_chatter, _vocalization_killed_enemy_needler, -1,
      _comm_protagonist_cause, _ai_communication_priority_filler, _comm_look_direction_protagonist, _ai_communication_priority_filler,
      10.0f, 0.0f, 0, -1, _comm_hostility_enemy, -1, -1, -1, _damage_category_needle },
    /* [ 11] _ai_communication_death */
    { _ai_communication_death, _ai_communication_priority_talk, _vocalization_killed_enemy_sniper, -1,
      _comm_protagonist_cause, _ai_communication_priority_filler, _comm_look_direction_protagonist, _ai_communication_priority_filler,
      20.0f, 0.0f, 0, -1, _comm_hostility_enemy, -1, -1, -1, _damage_category_sniper },
    /* [ 12] _ai_communication_death */
    { _ai_communication_death, _ai_communication_priority_talk, _vocalization_killed_enemy_grenade, -1,
      _comm_protagonist_cause, _ai_communication_priority_filler, _comm_look_direction_protagonist, _ai_communication_priority_filler,
      20.0f, 0.0f, 0, -1, _comm_hostility_enemy, -1, -1, -1, _damage_category_grenade },
    /* [ 13] _ai_communication_death */
    { _ai_communication_death, _ai_communication_priority_talk, _vocalization_killed_enemy_explosion, -1,
      _comm_protagonist_cause, _ai_communication_priority_filler, _comm_look_direction_protagonist, _ai_communication_priority_filler,
      20.0f, 0.0f, 0, -1, _comm_hostility_enemy, -1, -1, -1, _damage_category_highexplosive },
    /* [ 14] _ai_communication_death */
    { _ai_communication_death, _ai_communication_priority_talk, _vocalization_killed_enemy_melee, -1,
      _comm_protagonist_cause, _ai_communication_priority_filler, _comm_look_direction_protagonist, _ai_communication_priority_filler,
      20.0f, 0.0f, 0, -1, _comm_hostility_enemy, -1, -1, -1, _damage_category_melee },
    /* [ 15] _ai_communication_death */
    { _ai_communication_death, _ai_communication_priority_chatter, _vocalization_killed_enemy_flame, -1,
      _comm_protagonist_cause, _ai_communication_priority_filler, _comm_look_direction_protagonist, _ai_communication_priority_filler,
      20.0f, 0.0f, 0, -1, _comm_hostility_enemy, -1, -1, -1, _damage_category_flame },
    /* [ 16] _ai_communication_death */
    { _ai_communication_death, _ai_communication_priority_chatter, _vocalization_killed_enemy_shotgun, -1,
      _comm_protagonist_cause, _ai_communication_priority_filler, _comm_look_direction_protagonist, _ai_communication_priority_filler,
      20.0f, 0.0f, 0, -1, _comm_hostility_enemy, -1, -1, -1, _damage_category_shotgun },
    /* [ 17] _ai_communication_death */
    { _ai_communication_death, _ai_communication_priority_talk, _vocalization_killed_enemy_vehicle, -1,
      _comm_protagonist_cause, _ai_communication_priority_filler, _comm_look_direction_protagonist, _ai_communication_priority_filler,
      30.0f, 0.0f, 0, -1, _comm_hostility_enemy, -1, -1, -1, _damage_category_vehicle },
    /* [ 18] _ai_communication_death */
    { _ai_communication_death, _ai_communication_priority_chatter, _vocalization_killed_enemy_mountedweapon, -1,
      _comm_protagonist_cause, _ai_communication_priority_filler, _comm_look_direction_protagonist, _ai_communication_priority_filler,
      30.0f, 10.0f, 0, -1, _comm_hostility_enemy, -1, -1, -1, _damage_category_mountedweapon },
    /* [ 19] _ai_communication_death */
    { _ai_communication_death, _ai_communication_priority_chatter, _vocalization_friend_died, -1,
      _comm_protagonist_friend, _ai_communication_priority_filler, _comm_look_direction_target, _ai_communication_priority_filler,
      10.0f, 0.0f, 0, -1, -1, -1, -1, -1, -1 },
    /* [ 20] _ai_communication_death */
    { _ai_communication_death, _ai_communication_priority_yell, _vocalization_friend_player_died, -1,
      _comm_protagonist_friend, _ai_communication_priority_yell, _comm_look_direction_target, _ai_communication_priority_yell,
      15.0f, 0.0f, (1 << _dialogue_usage_force_bit) | (1 << _dialogue_usage_override_scripted_bit), -1, -1, -1, _race_player, -1, -1 },
    /* [ 21] _ai_communication_death */
    { _ai_communication_death, _ai_communication_priority_talk, _vocalization_friend_killed_by_friend, -1,
      _comm_protagonist_friend, _ai_communication_priority_shout, _comm_look_direction_subject, _ai_communication_priority_filler,
      10.0f, 0.0f, 0, -1, _comm_hostility_friend, -1, -1, -1, -1 },
    /* [ 22] _ai_communication_death */
    { _ai_communication_death, _ai_communication_priority_communicate, _vocalization_friend_killed_by_friend_player, -1,
      _comm_protagonist_friend, _ai_communication_priority_yell, _comm_look_direction_target, _ai_communication_priority_shout,
      20.0f, 0.0f, (1 << _dialogue_usage_force_bit) | (1 << _dialogue_usage_override_scripted_bit), -1, _comm_hostility_friend, -1, -1, _race_player, -1 },
    /* [ 23] _ai_communication_death */
    { _ai_communication_death, _ai_communication_priority_chatter, _vocalization_friend_killed_by_enemy, -1,
      _comm_protagonist_friend, _ai_communication_priority_filler, _comm_look_direction_target, _ai_communication_priority_filler,
      10.0f, 0.0f, 0, -1, _comm_hostility_enemy, -1, -1, -1, -1 },
    /* [ 24] _ai_communication_death */
    { _ai_communication_death, _ai_communication_priority_chatter, _vocalization_friend_killed_by_enemy_player, -1,
      _comm_protagonist_friend, _ai_communication_priority_filler, _comm_look_direction_target, _ai_communication_priority_filler,
      10.0f, 0.0f, 0, -1, _comm_hostility_enemy, -1, -1, _race_player, -1 },
    /* [ 25] _ai_communication_death */
    { _ai_communication_death, _ai_communication_priority_chatter, _vocalization_friend_killed_by_covenant, -1,
      _comm_protagonist_friend, _ai_communication_priority_filler, _comm_look_direction_target, _ai_communication_priority_filler,
      10.0f, 0.0f, 0, -1, _comm_hostility_enemy, -1, -1, _race_covenant, -1 },
    /* [ 26] _ai_communication_death */
    { _ai_communication_death, _ai_communication_priority_chatter, _vocalization_friend_killed_by_flood, -1,
      _comm_protagonist_friend, _ai_communication_priority_filler, _comm_look_direction_target, _ai_communication_priority_filler,
      40.0f, 0.0f, 0, -1, _comm_hostility_enemy, -1, -1, _race_flood, -1 },
    /* [ 27] _ai_communication_death */
    { _ai_communication_death, _ai_communication_priority_chatter, _vocalization_friend_killed_by_sentinel, -1,
      _comm_protagonist_friend, _ai_communication_priority_filler, _comm_look_direction_target, _ai_communication_priority_filler,
      40.0f, 0.0f, 0, -1, _comm_hostility_enemy, -1, -1, _race_sentinel, -1 },
    /* [ 28] _ai_communication_death */
    { _ai_communication_death, _ai_communication_priority_shout, _vocalization_friend_betrayed, -1,
      _comm_protagonist_friend, _ai_communication_priority_yell, _comm_look_direction_target, _ai_communication_priority_yell,
      30.0f, 0.0f, (1 << _dialogue_usage_force_bit), -1, _comm_hostility_traitor, -1, -1, -1, -1 },
    /* [ 29] _ai_communication_killing_spree */
    { _ai_communication_killing_spree, _ai_communication_priority_communicate, _vocalization_killing_spree, -1,
      _comm_protagonist_subject, _ai_communication_priority_filler, _comm_look_direction_protagonist, _ai_communication_priority_filler,
      30.0f, 0.0f, (1 << _dialogue_usage_player_bit), -1, -1, -1, -1, -1, -1 },
    /* [ 30] _ai_communication_damage */
    { _ai_communication_damage, _ai_communication_priority_talk, _vocalization_shot_friend, -1,
      _comm_protagonist_cause, _ai_communication_priority_filler, _comm_look_direction_protagonist, _ai_communication_priority_filler,
      10.0f, 0.0f, 0, -1, _comm_hostility_friend, -1, -1, -1, -1 },
    /* [ 31] _ai_communication_damage */
    { _ai_communication_damage, _ai_communication_priority_talk, _vocalization_shot_friend_player, -1,
      _comm_protagonist_cause, _ai_communication_priority_filler, _comm_look_direction_protagonist, _ai_communication_priority_filler,
      10.0f, 0.0f, (1 << _dialogue_usage_force_bit), -1, _comm_hostility_friend, -1, _race_player, -1, -1 },
    /* [ 32] _ai_communication_damage */
    { _ai_communication_damage, _ai_communication_priority_talk, _vocalization_hurt_friend, -1,
      _comm_protagonist_subject, _ai_communication_priority_shout, _comm_look_direction_target, _ai_communication_priority_filler,
      10.0f, 0.0f, (1 << _dialogue_usage_player_bit), -1, _comm_hostility_friend, -1, -1, -1, -1 },
    /* [ 33] _ai_communication_damage */
    { _ai_communication_damage, _ai_communication_priority_talk, _vocalization_hurt_friend_player, -1,
      _comm_protagonist_subject, _ai_communication_priority_yell, _comm_look_direction_target, _ai_communication_priority_filler,
      10.0f, 5.0f, 0, -1, _comm_hostility_friend, -1, -1, _race_player, -1 },
    /* [ 34] _ai_communication_hurt */
    { _ai_communication_hurt, _ai_communication_priority_filler, _vocalization_shot_enemy, -1,
      _comm_protagonist_cause, _ai_communication_priority_filler, _comm_look_direction_none, _ai_communication_priority_none,
      10.0f, 10.0f, 0, -1, _comm_hostility_enemy, -1, -1, -1, -1 },
    /* [ 35] _ai_communication_hurt */
    { _ai_communication_hurt, _ai_communication_priority_chatter, _vocalization_hurt_enemy, -1,
      _comm_protagonist_subject, _ai_communication_priority_shout, _comm_look_direction_none, _ai_communication_priority_none,
      10.0f, 10.0f, 0, -1, _comm_hostility_enemy, -1, -1, -1, -1 },
    /* [ 36] _ai_communication_hurt */
    { _ai_communication_hurt, _ai_communication_priority_chatter, _vocalization_hurt_enemy_bullet, -1,
      _comm_protagonist_subject, _ai_communication_priority_shout, _comm_look_direction_none, _ai_communication_priority_none,
      10.0f, 10.0f, 0, -1, _comm_hostility_enemy, -1, -1, -1, _damage_category_bullet },
    /* [ 37] _ai_communication_hurt */
    { _ai_communication_hurt, _ai_communication_priority_chatter, _vocalization_hurt_enemy_needler, -1,
      _comm_protagonist_subject, _ai_communication_priority_shout, _comm_look_direction_none, _ai_communication_priority_none,
      10.0f, 10.0f, 0, -1, _comm_hostility_enemy, -1, -1, -1, _damage_category_needle },
    /* [ 38] _ai_communication_hurt */
    { _ai_communication_hurt, _ai_communication_priority_chatter, _vocalization_hurt_enemy_plasma, -1,
      _comm_protagonist_subject, _ai_communication_priority_shout, _comm_look_direction_none, _ai_communication_priority_none,
      10.0f, 10.0f, 0, -1, _comm_hostility_enemy, -1, -1, -1, _damage_category_plasma },
    /* [ 39] _ai_communication_hurt */
    { _ai_communication_hurt, _ai_communication_priority_talk, _vocalization_hurt_enemy_sniper, -1,
      _comm_protagonist_subject, _ai_communication_priority_shout, _comm_look_direction_none, _ai_communication_priority_none,
      20.0f, 0.0f, 0, -1, _comm_hostility_enemy, -1, -1, -1, _damage_category_sniper },
    /* [ 40] _ai_communication_hurt */
    { _ai_communication_hurt, _ai_communication_priority_chatter, _vocalization_hurt_enemy_explosion, -1,
      _comm_protagonist_subject, _ai_communication_priority_shout, _comm_look_direction_none, _ai_communication_priority_none,
      20.0f, 0.0f, 0, -1, _comm_hostility_enemy, -1, -1, -1, _damage_category_highexplosive },
    /* [ 41] _ai_communication_hurt */
    { _ai_communication_hurt, _ai_communication_priority_talk, _vocalization_hurt_enemy_melee, -1,
      _comm_protagonist_subject, _ai_communication_priority_shout, _comm_look_direction_none, _ai_communication_priority_none,
      20.0f, 0.0f, 0, -1, _comm_hostility_enemy, -1, -1, -1, _damage_category_melee },
    /* [ 42] _ai_communication_hurt */
    { _ai_communication_hurt, _ai_communication_priority_chatter, _vocalization_hurt_enemy_flame, -1,
      _comm_protagonist_subject, _ai_communication_priority_shout, _comm_look_direction_none, _ai_communication_priority_none,
      20.0f, 0.0f, 0, -1, _comm_hostility_enemy, -1, -1, -1, _damage_category_flame },
    /* [ 43] _ai_communication_hurt */
    { _ai_communication_hurt, _ai_communication_priority_chatter, _vocalization_hurt_enemy_shotgun, -1,
      _comm_protagonist_subject, _ai_communication_priority_shout, _comm_look_direction_none, _ai_communication_priority_none,
      20.0f, 0.0f, 0, -1, _comm_hostility_enemy, -1, -1, -1, _damage_category_shotgun },
    /* [ 44] _ai_communication_hurt */
    { _ai_communication_hurt, _ai_communication_priority_talk, _vocalization_hurt_enemy_vehicle, -1,
      _comm_protagonist_subject, _ai_communication_priority_shout, _comm_look_direction_none, _ai_communication_priority_none,
      30.0f, 0.0f, 0, -1, _comm_hostility_enemy, -1, -1, -1, _damage_category_vehicle },
    /* [ 45] _ai_communication_hurt */
    { _ai_communication_hurt, _ai_communication_priority_chatter, _vocalization_hurt_enemy_mountedweapon, -1,
      _comm_protagonist_subject, _ai_communication_priority_shout, _comm_look_direction_none, _ai_communication_priority_none,
      30.0f, 0.0f, 0, -1, _comm_hostility_enemy, -1, -1, -1, _damage_category_mountedweapon },
    /* [ 46] _ai_communication_sighted_enemy */
    { _ai_communication_sighted_enemy, _ai_communication_priority_shout, _vocalization_sighted_enemy_new, _unit_animation_impulse_signal_warn,
      _comm_protagonist_subject, _ai_communication_priority_yell, _comm_look_direction_target, _ai_communication_priority_yell,
      10.0f, 10.0f, 0, -1, -1, _comm_enemy_never, -1, -1, -1 },
    /* [ 47] _ai_communication_sighted_enemy */
    { _ai_communication_sighted_enemy, _ai_communication_priority_shout, _vocalization_sighted_enemy_recent, _unit_animation_impulse_signal_warn,
      _comm_protagonist_subject, _ai_communication_priority_yell, _comm_look_direction_target, _ai_communication_priority_yell,
      10.0f, 25.0f, 0, -1, -1, _comm_enemy_lost, -1, -1, -1 },
    /* [ 48] _ai_communication_found_enemy */
    { _ai_communication_found_enemy, _ai_communication_priority_shout, _vocalization_sighted_enemy_searching, _unit_animation_impulse_signal_warn,
      _comm_protagonist_subject, _ai_communication_priority_yell, _comm_look_direction_target, _ai_communication_priority_yell,
      10.0f, 25.0f, 0, -1, -1, _comm_enemy_lost, -1, -1, -1 },
    /* [ 49] _ai_communication_unexpected_enemy */
    { _ai_communication_unexpected_enemy, _ai_communication_priority_talk, _vocalization_unexpected_enemy, -1,
      _comm_protagonist_subject, _ai_communication_priority_yell, _comm_look_direction_target, _ai_communication_priority_communicate,
      10.0f, 8.0f, (1 << _dialogue_usage_lookup_bit), -1, -1, -1, -1, -1, -1 },
    /* [ 50] _ai_communication_found_dead_friend */
    { _ai_communication_found_dead_friend, _ai_communication_priority_shout, _vocalization_dead_friend_found, -1,
      _comm_protagonist_subject, _ai_communication_priority_yell, _comm_look_direction_target, _ai_communication_priority_yell,
      10.0f, 20.0f, 0, -1, -1, _comm_enemy_not_dangerous, -1, -1, -1 },
    /* [ 51] _ai_communication_allegiance_changed */
    { _ai_communication_allegiance_changed, _ai_communication_priority_yell, _vocalization_allegiance_broken, -1,
      _comm_protagonist_subject, _ai_communication_priority_yell, _comm_look_direction_target, _ai_communication_priority_yell,
      10.0f, 0.0f, (1 << _dialogue_usage_immediate_notify_bit) | (1 << _dialogue_usage_override_scripted_bit), -1, _comm_hostility_traitor, -1, -1, -1, -1 },
    /* [ 52] _ai_communication_allegiance_changed */
    { _ai_communication_allegiance_changed, _ai_communication_priority_communicate, _vocalization_allegiance_reformed, -1,
      _comm_protagonist_subject, _ai_communication_priority_yell, _comm_look_direction_target, _ai_communication_priority_communicate,
      10.0f, 0.0f, (1 << _dialogue_usage_override_scripted_bit), -1, _comm_hostility_friend, -1, -1, -1, -1 },
    /* [ 53] _ai_communication_sighted_friend_player */
    { _ai_communication_sighted_friend_player, _ai_communication_priority_communicate, _vocalization_sighted_friend_player, _unit_animation_impulse_signal_warn,
      _comm_protagonist_subject, _ai_communication_priority_yell, _comm_look_direction_target, _ai_communication_priority_shout,
      10.0f, 15.0f, 0, -1, -1, _comm_enemy_not_dangerous, -1, -1, -1 },
    /* [ 54] _ai_communication_lost_contact */
    { _ai_communication_lost_contact, _ai_communication_priority_communicate, _vocalization_lost_contact, -1,
      _comm_protagonist_subject, _ai_communication_priority_none, _comm_look_direction_protagonist, _ai_communication_priority_filler,
      10.0f, 20.0f, 0, -1, -1, _comm_enemy_not_visible, -1, -1, -1 },
    /* [ 55] _ai_communication_lost_contact */
    { _ai_communication_lost_contact, _ai_communication_priority_communicate, _vocalization_alert_lost_contact, -1,
      _comm_protagonist_subject, _ai_communication_priority_none, _comm_look_direction_protagonist, _ai_communication_priority_filler,
      15.0f, 20.0f, 0, _comm_group_extended, -1, _comm_enemy_not_visible, -1, -1, -1 },
    /* [ 56] _ai_communication_alert_noncombat */
    { _ai_communication_alert_noncombat, _ai_communication_priority_communicate, _vocalization_alert_noncombat, -1,
      _comm_protagonist_subject, _ai_communication_priority_communicate, _comm_look_direction_protagonist, _ai_communication_priority_shout,
      10.0f, 20.0f, 0, -1, -1, _comm_enemy_lost, -1, -1, -1 },
    /* [ 57] _ai_communication_blocked */
    { _ai_communication_blocked, _ai_communication_priority_chatter, _vocalization_blocked, _unit_animation_impulse_signal_move,
      _comm_protagonist_subject, _ai_communication_priority_communicate, _comm_look_direction_protagonist, _ai_communication_priority_filler,
      10.0f, 0.0f, 0, _comm_group_tactical, -1, -1, -1, -1, _damage_category_none },
    /* [ 58] _ai_communication_search_start */
    { _ai_communication_search_start, _ai_communication_priority_talk, _vocalization_search_start, -1,
      _comm_protagonist_subject, _ai_communication_priority_none, _comm_look_direction_protagonist, _ai_communication_priority_filler,
      10.0f, 8.0f, 0, _comm_group_extended, -1, _comm_enemy_not_visible, -1, -1, -1 },
    /* [ 59] _ai_communication_search_query */
    { _ai_communication_search_query, _ai_communication_priority_talk, _vocalization_search_query, -1,
      _comm_protagonist_subject, _ai_communication_priority_shout, _comm_look_direction_protagonist, _ai_communication_priority_filler,
      10.0f, 25.0f, 0, _comm_group_extended, -1, _comm_enemy_not_visible, -1, -1, -1 },
    /* [ 60] _ai_communication_search_report */
    { _ai_communication_search_report, _ai_communication_priority_talk, _vocalization_search_report, -1,
      _comm_protagonist_subject, _ai_communication_priority_none, _comm_look_direction_protagonist, _ai_communication_priority_filler,
      10.0f, 25.0f, 0, _comm_group_extended, -1, _comm_enemy_not_visible, -1, -1, -1 },
    /* [ 61] _ai_communication_search_abandon */
    { _ai_communication_search_abandon, _ai_communication_priority_talk, _vocalization_search_abandon, -1,
      _comm_protagonist_subject, _ai_communication_priority_none, _comm_look_direction_protagonist, _ai_communication_priority_filler,
      10.0f, 8.0f, 0, _comm_group_extended, -1, _comm_enemy_not_visible, -1, -1, -1 },
    /* [ 62] _ai_communication_search_group_abandon */
    { _ai_communication_search_group_abandon, _ai_communication_priority_communicate, _vocalization_search_group_abandon, -1,
      _comm_protagonist_subject, _ai_communication_priority_none, _comm_look_direction_protagonist, _ai_communication_priority_filler,
      10.0f, 0.0f, 0, _comm_group_extended, -1, _comm_enemy_not_visible, -1, -1, -1 },
    /* [ 63] _ai_communication_uncover_start */
    { _ai_communication_uncover_start, _ai_communication_priority_talk, _vocalization_uncover_start, _unit_animation_impulse_signal_attack,
      _comm_protagonist_subject, _ai_communication_priority_none, _comm_look_direction_protagonist, _ai_communication_priority_filler,
      10.0f, 30.0f, 0, _comm_group_tactical, -1, _comm_enemy_not_visible, -1, -1, -1 },
    /* [ 64] _ai_communication_cover */
    { _ai_communication_cover, _ai_communication_priority_chatter, _vocalization_cover, -1,
      _comm_protagonist_subject, _ai_communication_priority_none, _comm_look_direction_protagonist, _ai_communication_priority_filler,
      10.0f, 30.0f, 0, _comm_group_tactical, -1, -1, -1, -1, -1 },
    /* [ 65] _ai_communication_advance */
    { _ai_communication_advance, _ai_communication_priority_communicate, _vocalization_advance, _unit_animation_impulse_signal_attack,
      _comm_protagonist_subject, _ai_communication_priority_none, _comm_look_direction_protagonist, _ai_communication_priority_filler,
      10.0f, 0.0f, 0, -1, -1, -1, -1, -1, -1 },
    /* [ 66] _ai_communication_retreat */
    { _ai_communication_retreat, _ai_communication_priority_communicate, _vocalization_retreat, -1,
      _comm_protagonist_subject, _ai_communication_priority_none, _comm_look_direction_protagonist, _ai_communication_priority_filler,
      10.0f, 0.0f, 0, -1, -1, -1, -1, -1, -1 },
    /* [ 67] _ai_communication_shooting */
    { _ai_communication_shooting, _ai_communication_priority_filler, _vocalization_shooting, -1,
      _comm_protagonist_subject, _ai_communication_priority_none, _comm_look_direction_none, _ai_communication_priority_none,
      10.0f, 10.0f, 0, -1, -1, -1, -1, -1, -1 },
    /* [ 68] _ai_communication_shooting_vehicle */
    { _ai_communication_shooting_vehicle, _ai_communication_priority_filler, _vocalization_shooting_vehicle, -1,
      _comm_protagonist_subject, _ai_communication_priority_none, _comm_look_direction_none, _ai_communication_priority_none,
      20.0f, 10.0f, 0, -1, -1, -1, -1, -1, -1 },
    /* [ 69] _ai_communication_shooting_berserk */
    { _ai_communication_shooting_berserk, _ai_communication_priority_chatter, _vocalization_shooting_berserk, -1,
      _comm_protagonist_subject, _ai_communication_priority_none, _comm_look_direction_none, _ai_communication_priority_none,
      20.0f, 0.0f, 0, -1, -1, -1, -1, -1, -1 },
    /* [ 70] _ai_communication_shooting_group */
    { _ai_communication_shooting_group, _ai_communication_priority_chatter, _vocalization_shooting_group, -1,
      _comm_protagonist_subject, _ai_communication_priority_none, _comm_look_direction_none, _ai_communication_priority_none,
      10.0f, 0.0f, 0, -1, -1, -1, -1, -1, -1 },
    /* [ 71] _ai_communication_shooting_traitor */
    { _ai_communication_shooting_traitor, _ai_communication_priority_filler, _vocalization_shooting_traitor, -1,
      _comm_protagonist_subject, _ai_communication_priority_none, _comm_look_direction_none, _ai_communication_priority_none,
      30.0f, 10.0f, 0, -1, -1, -1, -1, -1, -1 },
    /* [ 72] _ai_communication_flee */
    { _ai_communication_flee, _ai_communication_priority_talk, _vocalization_flee, -1,
      _comm_protagonist_subject, _ai_communication_priority_none, _comm_look_direction_protagonist, _ai_communication_priority_filler,
      10.0f, 0.0f, 0, -1, -1, -1, -1, -1, -1 },
    /* [ 73] _ai_communication_flee_leader_died */
    { _ai_communication_flee_leader_died, _ai_communication_priority_communicate, _vocalization_flee_leader_died, -1,
      _comm_protagonist_subject, _ai_communication_priority_none, _comm_look_direction_protagonist, _ai_communication_priority_filler,
      10.0f, 0.0f, 0, -1, -1, -1, -1, -1, -1 },
    /* [ 74] _ai_communication_flee_idle */
    { _ai_communication_flee_idle, _ai_communication_priority_talk, _vocalization_idle_flee, -1,
      _comm_protagonist_subject, _ai_communication_priority_none, _comm_look_direction_none, _ai_communication_priority_none,
      10.0f, 0.0f, 0, -1, -1, -1, -1, -1, -1 },
    /* [ 75] _ai_communication_attempted_flee */
    { _ai_communication_attempted_flee, _ai_communication_priority_chatter, _vocalization_attempted_flee, -1,
      _comm_protagonist_subject, _ai_communication_priority_communicate, _comm_look_direction_protagonist, _ai_communication_priority_shout,
      10.0f, 10.0f, 0, _comm_group_tactical, -1, -1, -1, -1, -1 },
    /* [ 76] _ai_communication_hiding_finished */
    { _ai_communication_hiding_finished, _ai_communication_priority_talk, _vocalization_hiding_finished, -1,
      _comm_protagonist_subject, _ai_communication_priority_none, _comm_look_direction_protagonist, _ai_communication_priority_filler,
      10.0f, 30.0f, 0, _comm_group_tactical, -1, _comm_enemy_not_visible, -1, -1, -1 },
    /* [ 77] _ai_communication_vehicle_entry */
    { _ai_communication_vehicle_entry, _ai_communication_priority_talk, _vocalization_vehicle_entry, -1,
      _comm_protagonist_subject, _ai_communication_priority_none, _comm_look_direction_none, _ai_communication_priority_none,
      10.0f, 10.0f, 0, -1, -1, -1, -1, -1, -1 },
    /* [ 78] _ai_communication_vehicle_exit */
    { _ai_communication_vehicle_exit, _ai_communication_priority_talk, _vocalization_vehicle_exit, -1,
      _comm_protagonist_subject, _ai_communication_priority_none, _comm_look_direction_none, _ai_communication_priority_none,
      10.0f, 10.0f, 0, -1, -1, -1, -1, -1, -1 },
    /* [ 79] _ai_communication_vehicle_woohoo */
    { _ai_communication_vehicle_woohoo, _ai_communication_priority_chatter, _vocalization_vehicle_woohoo, _unit_animation_impulse_vehicle_celebrate,
      _comm_protagonist_friend, _ai_communication_priority_none, _comm_look_direction_none, _ai_communication_priority_none,
      10.0f, 25.0f, (1 << _dialogue_usage_same_vehicle_bit) | (1 << _dialogue_usage_allow_subject_bit), -1, -1, -1, -1, -1, -1 },
    /* [ 80] _ai_communication_vehicle_scared */
    { _ai_communication_vehicle_scared, _ai_communication_priority_chatter, _vocalization_vehicle_scared, _unit_animation_impulse_vehicle_panic,
      _comm_protagonist_friend, _ai_communication_priority_none, _comm_look_direction_none, _ai_communication_priority_none,
      10.0f, 25.0f, (1 << _dialogue_usage_same_vehicle_bit) | (1 << _dialogue_usage_allow_subject_bit), -1, -1, -1, -1, -1, -1 },
    /* [ 81] _ai_communication_vehicle_falling */
    { _ai_communication_vehicle_falling, _ai_communication_priority_exclaim, _vocalization_scream_fear, _unit_animation_impulse_vehicle_panic,
      _comm_protagonist_friend, _ai_communication_priority_none, _comm_look_direction_none, _ai_communication_priority_none,
      10.0f, 0.0f, (1 << _dialogue_usage_same_vehicle_bit) | (1 << _dialogue_usage_allow_subject_bit), -1, -1, -1, -1, -1, -1 },
    /* [ 82] _ai_communication_grenade_sighted */
    { _ai_communication_grenade_sighted, _ai_communication_priority_talk, _vocalization_grenade_sighted, -1,
      _comm_protagonist_subject, _ai_communication_priority_yell, _comm_look_direction_danger, _ai_communication_priority_yell,
      10.0f, 4.0f, 0, _comm_group_extended, _comm_hostility_enemy, -1, -1, -1, -1 },
    /* [ 83] _ai_communication_grenade_startle */
    { _ai_communication_grenade_startle, _ai_communication_priority_talk, _vocalization_grenade_startle, -1,
      _comm_protagonist_subject, _ai_communication_priority_yell, _comm_look_direction_danger, _ai_communication_priority_yell,
      10.0f, 4.0f, 0, -1, -1, _comm_enemy_not_dangerous, -1, -1, -1 },
    /* [ 84] _ai_communication_grenade_danger */
    { _ai_communication_grenade_danger, _ai_communication_priority_shout, _vocalization_grenade_danger_enemy, -1,
      _comm_protagonist_subject, _ai_communication_priority_shout, _comm_look_direction_danger, _ai_communication_priority_shout,
      10.0f, 4.0f, 0, _comm_group_extended, _comm_hostility_enemy, -1, -1, -1, -1 },
    /* [ 85] _ai_communication_grenade_danger */
    { _ai_communication_grenade_danger, _ai_communication_priority_shout, _vocalization_grenade_danger_friend, -1,
      _comm_protagonist_subject, _ai_communication_priority_shout, _comm_look_direction_danger, _ai_communication_priority_shout,
      10.0f, 0.0f, 0, _comm_group_extended, _comm_hostility_friend, -1, -1, -1, -1 },
    /* [ 86] _ai_communication_grenade_danger */
    { _ai_communication_grenade_danger, _ai_communication_priority_yell, _vocalization_grenade_danger_self, -1,
      _comm_protagonist_subject, _ai_communication_priority_yell, _comm_look_direction_danger, _ai_communication_priority_shout,
      10.0f, 0.0f, 0, -1, _comm_hostility_self, -1, -1, -1, -1 },
    /* [ 87] _ai_communication_surprise */
    { _ai_communication_surprise, _ai_communication_priority_exclaim, _vocalization_surprise, -1,
      _comm_protagonist_subject, _ai_communication_priority_yell, _comm_look_direction_protagonist, _ai_communication_priority_communicate,
      10.0f, 0.0f, 0, -1, -1, -1, -1, -1, -1 },
    /* [ 88] _ai_communication_berserk */
    { _ai_communication_berserk, _ai_communication_priority_exclaim, _vocalization_berserk, -1,
      _comm_protagonist_subject, _ai_communication_priority_communicate, _comm_look_direction_protagonist, _ai_communication_priority_filler,
      10.0f, 0.0f, 0, -1, -1, -1, -1, -1, -1 },
    /* [ 89] _ai_communication_melee */
    { _ai_communication_melee, _ai_communication_priority_exclaim, _vocalization_melee, -1,
      _comm_protagonist_subject, _ai_communication_priority_communicate, _comm_look_direction_protagonist, _ai_communication_priority_filler,
      10.0f, 0.0f, 0, -1, -1, -1, -1, -1, -1 },
    /* [ 90] _ai_communication_grenade_throwing */
    { _ai_communication_grenade_throwing, _ai_communication_priority_exclaim, _vocalization_grenade_throwing, -1,
      _comm_protagonist_subject, _ai_communication_priority_communicate, _comm_look_direction_protagonist, _ai_communication_priority_filler,
      10.0f, 0.0f, 0, -1, -1, -1, -1, -1, -1 },
    /* [ 91] _ai_communication_dive */
    { _ai_communication_dive, _ai_communication_priority_exclaim, _vocalization_dive, -1,
      _comm_protagonist_subject, _ai_communication_priority_communicate, _comm_look_direction_protagonist, _ai_communication_priority_filler,
      10.0f, 0.0f, 0, -1, -1, -1, -1, -1, -1 },
    /* [ 92] _ai_communication_leap */
    { _ai_communication_leap, _ai_communication_priority_exclaim, _vocalization_leap, -1,
      _comm_protagonist_subject, _ai_communication_priority_communicate, _comm_look_direction_protagonist, _ai_communication_priority_filler,
      10.0f, 0.0f, 0, -1, -1, -1, -1, -1, -1 },
    /* [ 93] _ai_communication_falling_to_death */
    { _ai_communication_falling_to_death, _ai_communication_priority_exclaim, _vocalization_scream_fear, -1,
      _comm_protagonist_subject, _ai_communication_priority_communicate, _comm_look_direction_protagonist, _ai_communication_priority_filler,
      10.0f, 0.0f, 0, -1, -1, -1, -1, -1, -1 },
    /* [ 94] _ai_communication_postcombat_alone */
    { _ai_communication_postcombat_alone, _ai_communication_priority_talk, _vocalization_postcombat_alone, -1,
      _comm_protagonist_subject, _ai_communication_priority_shout, _comm_look_direction_none, _ai_communication_priority_none,
      10.0f, 0.0f, 0, -1, -1, -1, -1, -1, -1 },
    /* [ 95] _ai_communication_postcombat_unscathed */
    { _ai_communication_postcombat_unscathed, _ai_communication_priority_talk, _vocalization_postcombat_unscathed, -1,
      _comm_protagonist_subject, _ai_communication_priority_none, _comm_look_direction_none, _ai_communication_priority_none,
      10.0f, 0.0f, 0, -1, -1, -1, -1, -1, -1 },
    /* [ 96] _ai_communication_postcombat_wounded */
    { _ai_communication_postcombat_wounded, _ai_communication_priority_talk, _vocalization_postcombat_wounded, -1,
      _comm_protagonist_subject, _ai_communication_priority_none, _comm_look_direction_none, _ai_communication_priority_none,
      10.0f, 0.0f, 0, -1, -1, -1, -1, -1, -1 },
    /* [ 97] _ai_communication_postcombat_massacre */
    { _ai_communication_postcombat_massacre, _ai_communication_priority_talk, _vocalization_postcombat_massacre, -1,
      _comm_protagonist_subject, _ai_communication_priority_none, _comm_look_direction_none, _ai_communication_priority_none,
      10.0f, 0.0f, 0, -1, -1, -1, -1, -1, -1 },
    /* [ 98] _ai_communication_postcombat_triumph */
    { _ai_communication_postcombat_triumph, _ai_communication_priority_talk, _vocalization_postcombat_triumph, -1,
      _comm_protagonist_subject, _ai_communication_priority_none, _comm_look_direction_none, _ai_communication_priority_none,
      10.0f, 0.0f, 0, -1, -1, -1, -1, -1, -1 },
    /* [ 99] _ai_communication_postcombat_check_enemy */
    { _ai_communication_postcombat_check_enemy, _ai_communication_priority_talk, _vocalization_check_body_enemy, -1,
      _comm_protagonist_subject, _ai_communication_priority_communicate, _comm_look_direction_protagonist, _ai_communication_priority_filler,
      10.0f, 0.0f, 0, -1, _comm_hostility_enemy, -1, -1, -1, -1 },
    /* [100] _ai_communication_postcombat_check_friend */
    { _ai_communication_postcombat_check_friend, _ai_communication_priority_talk, _vocalization_check_body_friend, -1,
      _comm_protagonist_subject, _ai_communication_priority_communicate, _comm_look_direction_protagonist, _ai_communication_priority_filler,
      10.0f, 0.0f, 0, -1, _comm_hostility_friend, -1, -1, -1, -1 },
    /* [101] _ai_communication_postcombat_shoot_corpse */
    { _ai_communication_postcombat_shoot_corpse, _ai_communication_priority_talk, _vocalization_shoot_corpse, -1,
      _comm_protagonist_subject, _ai_communication_priority_communicate, _comm_look_direction_protagonist, _ai_communication_priority_filler,
      10.0f, 0.0f, 0, -1, -1, -1, -1, -1, -1 },
    /* [102] _ai_communication_postcombat_shoot_corpse */
    { _ai_communication_postcombat_shoot_corpse, _ai_communication_priority_yell, _vocalization_shoot_corpse_player, -1,
      _comm_protagonist_subject, _ai_communication_priority_communicate, _comm_look_direction_protagonist, _ai_communication_priority_filler,
      10.0f, 0.0f, (1 << _dialogue_usage_force_bit), -1, -1, -1, -1, _race_player, -1 },
    /* [103] _ai_communication_postcombat_celebrate */
    { _ai_communication_postcombat_celebrate, _ai_communication_priority_talk, _vocalization_celebrate, -1,
      _comm_protagonist_subject, _ai_communication_priority_communicate, _comm_look_direction_protagonist, _ai_communication_priority_filler,
      10.0f, 0.0f, 0, -1, -1, -1, -1, -1, -1 },
    /* [104] terminator */
    { -1, -1, -1, -1,
      -1, -1, -1, -1,
      0.0f, 0.0f, 0, -1, -1, -1, -1, -1, -1 },
};
