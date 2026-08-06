/* global_reply_table @ 0x82127A08 (.rdata, 1656 bytes)
 * DB applied_types: const reply_usage global_reply_table[46];
 * Image bytes (big-endian), decoded from the binary .rdata record:
 *   [ 0] +0x00 original_vocalization_type = 0x004C
 *        +0x02 original_damage_category   = 0xFFFF
 *        +0x04 protagonist_type           = 0x0002
 *        +0x06 vocalization_type          = 0x005C
 *        +0x08 animation_type             = 0xFFFF
 *        +0x0A communication_priority     = 0x0002
 *        +0x0C flags                      = 0x0000
 *        +0x10 chance                     = 0x00000000 -> 0f
 *        +0x14 player_chance              = 0x3F800000 -> 1f
 *        +0x18 delay_time                 = 0x3F333333 -> 0.7f
 *        +0x1C repeat_delay               = 0x41F00000 -> 30f
 *        +0x20 reply_filter               = 0x837CBF60 -> reply_filter_close
 *   [ 1] +0x00 original_vocalization_type = 0x0035
 *        +0x02 original_damage_category   = 0x0002
 *        +0x04 protagonist_type           = 0x0002
 *        +0x06 vocalization_type          = 0x0051
 *        +0x08 animation_type             = 0xFFFF
 *        +0x0A communication_priority     = 0x0002
 *        +0x0C flags                      = 0x0000
 *        +0x10 chance                     = 0x00000000 -> 0f
 *        +0x14 player_chance              = 0x3F000000 -> 0.5f
 *        +0x18 delay_time                 = 0x3F333333 -> 0.7f
 *        +0x1C repeat_delay               = 0x42700000 -> 60f
 *        +0x20 reply_filter               = 0x837CBF60 -> reply_filter_close
 *   [ 2] +0x00 original_vocalization_type = 0x0035
 *        +0x02 original_damage_category   = 0x000B
 *        +0x04 protagonist_type           = 0x0002
 *        +0x06 vocalization_type          = 0x0052
 *        +0x08 animation_type             = 0xFFFF
 *        +0x0A communication_priority     = 0x0002
 *        +0x0C flags                      = 0x0000
 *        +0x10 chance                     = 0x00000000 -> 0f
 *        +0x14 player_chance              = 0x3F19999A -> 0.6f
 *        +0x18 delay_time                 = 0x3F333333 -> 0.7f
 *        +0x1C repeat_delay               = 0x42700000 -> 60f
 *        +0x20 reply_filter               = 0x837CBF60 -> reply_filter_close
 *   ... 42 further elements elided; full hex in .sweep/data_image.tsv
 *   [45] +0x00 original_vocalization_type = 0xFFFF
 *        +0x02 original_damage_category   = 0xFFFF
 *        +0x04 protagonist_type           = 0xFFFF
 *        +0x06 vocalization_type          = 0xFFFF
 *        +0x08 animation_type             = 0xFFFF
 *        +0x0A communication_priority     = 0xFFFF
 *        +0x0C flags                      = 0x0000
 *        +0x10 chance                     = 0x00000000 -> 0f
 *        +0x14 player_chance              = 0x00000000 -> 0f
 *        +0x18 delay_time                 = 0x00000000 -> 0f
 *        +0x1C repeat_delay               = 0x00000000 -> 0f
 *        +0x20 reply_filter               = 0x00000000
 * The AI reply table: for a line that was
 * just spoken (original_vocalization_type, and optionally the damage category that caused it),
 * every reply another actor may voice, with the chance/delay and the eligibility filter applied to
 * the candidate replier. Rows are grouped by original_vocalization_type, which is what lets
 * ai_communication_finished and ai_communication_find_actor_to_reply_to_player walk the contiguous
 * run for one vocalization.
 * Element type and length are PROVEN, not inferred from the (contradictory) corpus declarations:
 * ai_communication_initialize @0x837CBA18 forms the object address directly (`lis r11,
 * global_reply_table@ha` / `addi r10, r11, global_reply_table@l` — no pointer load) and strides it
 * with `lhzu r9, 0x24(r10)`; ai_communication_update_speech_timers computes `36 * index` and loads
 * +0x1C (repeat_delay). 0x24 == 36 == sizeof(reply_usage) and 1656 / 36 == 46 rows exactly.
 * Row [45] is the terminator (original_vocalization_type == -1); the initialize loop counts up to
 * it, yielding global_reply_event_count == 45.
 * Field order per row (reply_usage, DB-verified layout):
 *   original_vocalization_type, original_damage_category, protagonist_type, vocalization_type,
 *   animation_type, communication_priority, flags, _pad0E,
 *   chance, player_chance, delay_time, repeat_delay, reply_filter
 * -1 in any int16_t field is the "no requirement / none" sentinel; a null reply_filter means the
 * reply has no extra eligibility test.
 */
#include <stdint.h>
#include "../headers/reply_usage.h"
#include "../headers/ai_information_packet.h"
#include "../headers/ai_communication_priority.h"
#include "../headers/ai_communication_protagonist_type.h"
#include "../headers/ai_vocalization_type.h"
#include "../headers/unit_animation_impulse.h"
#include "../headers/reply_usage_override_scripted_flags.h"
#include "../headers/damage_category.h"

/* The filter externs carry the funcptr-SLOT signature from reply_usage +0x20, not each function's
 * own attested one — they exist only to take an address for this table. reply_filter_fighting_close
 * and reply_filter_same_target spell their return `int` in their own TUs; the slot is uint8_t. */
extern uint8_t reply_filter_close(int original_unit_index, ai_information_packet *communication, int reply_actor_index);
extern uint8_t reply_filter_fighting(int original_unit_index, ai_information_packet *communication, int reply_actor_index);
extern uint8_t reply_filter_fighting_close(int original_unit_index, ai_information_packet *communication, int reply_actor_index);
extern uint8_t reply_filter_flee_leader(int original_unit_index, ai_information_packet *communication, int reply_actor_index);
extern uint8_t reply_filter_no_certain_target(int original_unit_index, ai_information_packet *communication, int reply_actor_index);
extern uint8_t reply_filter_not_close(int original_unit_index, ai_information_packet *communication, int reply_actor_index);
extern uint8_t reply_filter_same_platoon(int original_unit_index, ai_information_packet *communication, int reply_actor_index);
extern uint8_t reply_filter_same_target(int original_unit_index, ai_information_packet *communication, int reply_actor_index);
extern uint8_t reply_filter_searching(int original_unit_index, ai_information_packet *communication, int reply_actor_index);

const reply_usage global_reply_table[46] =
{
    /* [ 0] reply to _vocalization_killing_spree */
    { _vocalization_killing_spree, -1, _comm_protagonist_friend, _vocalization_player_killing_spree_comment,
      -1, _ai_communication_priority_chatter, 0, 0,
      0.0f, 1.0f, 0.7f, 30.0f, reply_filter_close },
    /* [ 1] reply to _vocalization_killed_enemy */
    { _vocalization_killed_enemy, _damage_category_bullet, _comm_protagonist_friend, _vocalization_player_kill_bullet_comment,
      -1, _ai_communication_priority_chatter, 0, 0,
      0.0f, 0.5f, 0.7f, 60.0f, reply_filter_close },
    /* [ 2] reply to _vocalization_killed_enemy */
    { _vocalization_killed_enemy, _damage_category_needle, _comm_protagonist_friend, _vocalization_player_kill_needler_comment,
      -1, _ai_communication_priority_chatter, 0, 0,
      0.0f, 0.6f, 0.7f, 60.0f, reply_filter_close },
    /* [ 3] reply to _vocalization_killed_enemy */
    { _vocalization_killed_enemy, _damage_category_plasma, _comm_protagonist_friend, _vocalization_player_kill_plasma_comment,
      -1, _ai_communication_priority_chatter, 0, 0,
      0.0f, 0.6f, 0.7f, 60.0f, reply_filter_close },
    /* [ 4] reply to _vocalization_killed_enemy */
    { _vocalization_killed_enemy, _damage_category_sniper, _comm_protagonist_friend, _vocalization_player_kill_sniper_comment,
      -1, _ai_communication_priority_chatter, 0, 0,
      0.0f, 0.8f, 0.7f, 60.0f, reply_filter_close },
    /* [ 5] reply to _vocalization_killed_enemy */
    { _vocalization_killed_enemy, _damage_category_grenade, _comm_protagonist_friend, _vocalization_player_kill_grenade_comment,
      -1, _ai_communication_priority_chatter, 0, 0,
      0.5f, 0.9f, 0.7f, 60.0f, reply_filter_close },
    /* [ 6] reply to _vocalization_killed_enemy */
    { _vocalization_killed_enemy, _damage_category_highexplosive, _comm_protagonist_friend, _vocalization_player_kill_explosion_comment,
      -1, _ai_communication_priority_chatter, 0, 0,
      0.0f, 1.0f, 0.7f, 60.0f, reply_filter_close },
    /* [ 7] reply to _vocalization_killed_enemy */
    { _vocalization_killed_enemy, _damage_category_melee, _comm_protagonist_friend, _vocalization_player_kill_melee_comment,
      -1, _ai_communication_priority_chatter, 0, 0,
      0.0f, 1.0f, 0.7f, 60.0f, reply_filter_close },
    /* [ 8] reply to _vocalization_killed_enemy */
    { _vocalization_killed_enemy, _damage_category_flame, _comm_protagonist_friend, _vocalization_player_kill_flame_comment,
      -1, _ai_communication_priority_chatter, 0, 0,
      0.0f, 0.9f, 0.7f, 60.0f, reply_filter_close },
    /* [ 9] reply to _vocalization_killed_enemy */
    { _vocalization_killed_enemy, _damage_category_shotgun, _comm_protagonist_friend, _vocalization_player_kill_shotgun_comment,
      -1, _ai_communication_priority_chatter, 0, 0,
      0.0f, 0.6f, 0.7f, 60.0f, reply_filter_close },
    /* [10] reply to _vocalization_killed_enemy */
    { _vocalization_killed_enemy, _damage_category_vehicle, _comm_protagonist_friend, _vocalization_player_kill_vehicle_comment,
      -1, _ai_communication_priority_chatter, 0, 0,
      0.0f, 0.8f, 0.7f, 60.0f, reply_filter_close },
    /* [11] reply to _vocalization_killed_enemy */
    { _vocalization_killed_enemy, _damage_category_mountedweapon, _comm_protagonist_friend, _vocalization_player_kill_mountedweapon_comment,
      -1, _ai_communication_priority_chatter, 0, 0,
      0.0f, 1.0f, 0.7f, 60.0f, reply_filter_close },
    /* [12] reply to _vocalization_killed_enemy_player */
    { _vocalization_killed_enemy_player, -1, _comm_protagonist_friend, _vocalization_killed_enemy_player_comment,
      -1, _ai_communication_priority_communicate, (1 << _reply_usage_override_scripted_bit), 0,
      1.0f, 1.0f, 0.3f, 0.0f, reply_filter_close },
    /* [13] reply to _vocalization_killed_enemy_covenant */
    { _vocalization_killed_enemy_covenant, -1, _comm_protagonist_friend, _vocalization_killed_enemy_covenant_comment,
      -1, _ai_communication_priority_chatter, 0, 0,
      0.8f, 0.6f, 0.5f, 30.0f, reply_filter_close },
    /* [14] reply to _vocalization_killed_enemy_floodcombat */
    { _vocalization_killed_enemy_floodcombat, -1, _comm_protagonist_friend, _vocalization_killed_enemy_floodcombat_comment,
      -1, _ai_communication_priority_chatter, 0, 0,
      0.8f, 0.6f, 0.5f, 20.0f, reply_filter_close },
    /* [15] reply to _vocalization_killed_enemy_floodcarrier */
    { _vocalization_killed_enemy_floodcarrier, -1, _comm_protagonist_friend, _vocalization_killed_enemy_floodcarrier_comment,
      -1, _ai_communication_priority_chatter, 0, 0,
      0.8f, 0.6f, 0.5f, 20.0f, reply_filter_close },
    /* [16] reply to _vocalization_killed_enemy_sentinel */
    { _vocalization_killed_enemy_sentinel, -1, _comm_protagonist_friend, _vocalization_killed_enemy_sentinel_comment,
      -1, _ai_communication_priority_chatter, 0, 0,
      0.8f, 0.6f, 0.5f, 20.0f, reply_filter_close },
    /* [17] reply to _vocalization_killed_enemy */
    { _vocalization_killed_enemy, -1, _comm_protagonist_friend, _vocalization_killed_enemy_comment,
      -1, _ai_communication_priority_chatter, 0, 0,
      0.6f, 0.4f, 0.5f, 30.0f, reply_filter_close },
    /* [18] reply to _vocalization_killed_enemy */
    { _vocalization_killed_enemy, -1, _comm_protagonist_friend, _vocalization_player_kill_comment,
      -1, _ai_communication_priority_chatter, 0, 0,
      0.0f, 0.4f, 0.7f, 40.0f, reply_filter_close },
    /* [19] reply to _vocalization_shot_enemy */
    { _vocalization_shot_enemy, -1, _comm_protagonist_friend, _vocalization_shot_enemy_comment,
      -1, _ai_communication_priority_filler, 0, 0,
      0.8f, 0.0f, 0.7f, 20.0f, 0 },
    /* [20] reply to _vocalization_hurt_enemy */
    { _vocalization_hurt_enemy, -1, _comm_protagonist_enemy, _vocalization_hurt_enemy_reply,
      -1, _ai_communication_priority_filler, 0, 0,
      0.8f, 0.0f, 0.7f, 20.0f, 0 },
    /* [21] reply to _vocalization_hurt_enemy */
    { _vocalization_hurt_enemy, -1, _comm_protagonist_friend, _vocalization_hurt_enemy_comment,
      -1, _ai_communication_priority_filler, 0, 0,
      0.8f, 0.0f, 0.7f, 20.0f, 0 },
    /* [22] reply to _vocalization_killed_friend_player */
    { _vocalization_killed_friend_player, -1, _comm_protagonist_friend, _vocalization_killed_friend_player_comment,
      -1, _ai_communication_priority_communicate, (1 << _reply_usage_override_scripted_bit), 0,
      1.0f, 0.0f, 0.3f, 0.0f, reply_filter_close },
    /* [23] reply to _vocalization_killed_friend */
    { _vocalization_killed_friend, -1, _comm_protagonist_friend, _vocalization_killed_friend_comment,
      -1, _ai_communication_priority_talk, 0, 0,
      0.7f, 0.0f, 0.3f, 20.0f, reply_filter_close },
    /* [24] reply to _vocalization_hurt_friend */
    { _vocalization_hurt_friend, -1, _comm_protagonist_target, _vocalization_hurt_friend_reply,
      -1, _ai_communication_priority_chatter, 0, 0,
      0.7f, 0.4f, 0.5f, 20.0f, 0 },
    /* [25] reply to _vocalization_sighted_enemy_new */
    { _vocalization_sighted_enemy_new, -1, _comm_protagonist_friend, _vocalization_sighted_enemy_near_reply,
      -1, _ai_communication_priority_talk, 0, 0,
      0.8f, 0.0f, 0.7f, 20.0f, reply_filter_close },
    /* [26] reply to _vocalization_sighted_enemy_new */
    { _vocalization_sighted_enemy_new, -1, _comm_protagonist_friend, _vocalization_sighted_enemy_far_reply,
      -1, _ai_communication_priority_talk, 0, 0,
      0.8f, 0.0f, 0.7f, 20.0f, reply_filter_not_close },
    /* [27] reply to _vocalization_sighted_enemy_recent */
    { _vocalization_sighted_enemy_recent, -1, _comm_protagonist_friend, _vocalization_sighted_enemy_near_reply,
      -1, _ai_communication_priority_talk, 0, 0,
      0.8f, 0.0f, 0.7f, 20.0f, reply_filter_close },
    /* [28] reply to _vocalization_sighted_enemy_recent */
    { _vocalization_sighted_enemy_recent, -1, _comm_protagonist_friend, _vocalization_sighted_enemy_far_reply,
      -1, _ai_communication_priority_talk, 0, 0,
      0.8f, 0.0f, 0.7f, 20.0f, reply_filter_not_close },
    /* [29] reply to _vocalization_sighted_enemy_searching */
    { _vocalization_sighted_enemy_searching, -1, _comm_protagonist_friend, _vocalization_sighted_enemy_near_reply,
      -1, _ai_communication_priority_talk, 0, 0,
      0.8f, 0.0f, 0.7f, 20.0f, reply_filter_close },
    /* [30] reply to _vocalization_sighted_enemy_searching */
    { _vocalization_sighted_enemy_searching, -1, _comm_protagonist_friend, _vocalization_sighted_enemy_far_reply,
      -1, _ai_communication_priority_talk, 0, 0,
      0.8f, 0.0f, 0.7f, 20.0f, reply_filter_not_close },
    /* [31] reply to _vocalization_alert_noncombat */
    { _vocalization_alert_noncombat, -1, _comm_protagonist_target, _vocalization_alert_noncombat_reply,
      -1, _ai_communication_priority_talk, 0, 0,
      0.7f, 0.0f, 0.5f, 15.0f, 0 },
    /* [32] reply to _vocalization_alert_lost_contact */
    { _vocalization_alert_lost_contact, -1, _comm_protagonist_friend, _vocalization_alert_lost_contact_reply,
      -1, _ai_communication_priority_talk, 0, 0,
      0.7f, 0.0f, 0.5f, 30.0f, reply_filter_no_certain_target },
    /* [33] reply to _vocalization_blocked */
    { _vocalization_blocked, -1, _comm_protagonist_target, _vocalization_blocked_reply,
      -1, _ai_communication_priority_talk, 0, 0,
      0.5f, 0.0f, 0.5f, 20.0f, 0 },
    /* [34] reply to _vocalization_search_query */
    { _vocalization_search_query, -1, _comm_protagonist_friend, _vocalization_search_query_reply,
      -1, _ai_communication_priority_talk, 0, 0,
      1.0f, 0.0f, 0.3f, 20.0f, reply_filter_searching },
    /* [35] reply to _vocalization_uncover_start */
    { _vocalization_uncover_start, -1, _comm_protagonist_friend, _vocalization_uncover_start_reply,
      -1, _ai_communication_priority_talk, 0, 0,
      1.0f, 0.0f, 0.3f, 20.0f, reply_filter_same_target },
    /* [36] reply to _vocalization_advance */
    { _vocalization_advance, -1, _comm_protagonist_friend, _vocalization_advance_reply,
      -1, _ai_communication_priority_communicate, 0, 0,
      0.7f, 0.0f, 0.7f, 20.0f, reply_filter_same_platoon },
    /* [37] reply to _vocalization_retreat */
    { _vocalization_retreat, -1, _comm_protagonist_friend, _vocalization_retreat_reply,
      -1, _ai_communication_priority_communicate, 0, 0,
      0.7f, 0.0f, 0.7f, 20.0f, reply_filter_same_platoon },
    /* [38] reply to _vocalization_flee */
    { _vocalization_flee, -1, _comm_protagonist_friend, _vocalization_flee_reply,
      -1, _ai_communication_priority_talk, 0, 0,
      0.5f, 0.0f, 0.7f, 30.0f, reply_filter_fighting_close },
    /* [39] reply to _vocalization_flee */
    { _vocalization_flee, -1, _comm_protagonist_enemy, _vocalization_taunt,
      -1, _ai_communication_priority_talk, 0, 0,
      0.5f, 0.0f, 0.7f, 30.0f, reply_filter_fighting },
    /* [40] reply to _vocalization_flee_leader_died */
    { _vocalization_flee_leader_died, -1, _comm_protagonist_enemy, _vocalization_taunt,
      -1, _ai_communication_priority_talk, 0, 0,
      0.5f, 0.0f, 0.7f, 30.0f, reply_filter_fighting },
    /* [41] reply to _vocalization_attempted_flee */
    { _vocalization_attempted_flee, -1, _comm_protagonist_friend, _vocalization_attempted_flee_reply,
      -1, _ai_communication_priority_talk, 0, 0,
      0.5f, 0.0f, 0.7f, 20.0f, reply_filter_flee_leader },
    /* [42] reply to _vocalization_postcombat_wounded */
    { _vocalization_postcombat_wounded, -1, _comm_protagonist_friend, _vocalization_postcombat_wounded_reply,
      -1, _ai_communication_priority_talk, 0, 0,
      0.8f, 0.0f, 0.5f, 0.0f, 0 },
    /* [43] reply to _vocalization_postcombat_massacre */
    { _vocalization_postcombat_massacre, -1, _comm_protagonist_friend, _vocalization_postcombat_massacre_reply,
      -1, _ai_communication_priority_talk, 0, 0,
      0.8f, 0.0f, 0.5f, 0.0f, 0 },
    /* [44] reply to _vocalization_postcombat_triumph */
    { _vocalization_postcombat_triumph, -1, _comm_protagonist_friend, _vocalization_postcombat_triumph_reply,
      -1, _ai_communication_priority_talk, 0, 0,
      0.8f, 0.0f, 0.5f, 0.0f, 0 },
    /* [45] terminator */
    { -1, -1, -1, -1,
      -1, -1, 0, 0,
      0.0f, 0.0f, 0.0f, 0.0f, 0 },
};
