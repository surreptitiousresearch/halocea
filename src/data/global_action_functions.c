/* global_action_functions @ 0x821286C8 — .rdata, 784 bytes = 14 x action_specification
 * (sizeof == 56; 784 / 56 == 14 == number_of_actor_actions, no padding). The actor-action dispatch
 * table: one row per actor_action, carrying the action id, its console/debug name and debug color,
 * the size of its action_state_data payload, its action_class, and the nine per-action callbacks.
 * Initializer reconstructed from the binary relocations and big-endian words; every non-null
 * pointer word below was resolved to a DB symbol, every null word is an absent callback:
 *
 *   row  action    name        color                        data_size  action_class
 *   [0]  none      "none"      global_real_argb_black         0x00     noncombat   (no callbacks)
 *   [1]  sleep     "sleep"     global_real_argb_lightblue     0x00     noncombat   control
 *   [2]  alert     "alert"     global_real_argb_cyan          0x5C     noncombat   begin perform update control flush_pos flush_struct
 *   [3]  fight     "fight"     global_real_argb_white         0x04     active      begin perform update control end
 *   [4]  flee      "flee"      global_real_argb_yellow        0x30     transitory  begin perform update control end modify_color replace_prop flush_pos
 *   [5]  uncover   "uncover"   global_real_argb_blue          0x34     pursuit     begin perform update control modify_color flush_pos
 *   [6]  guard     "guard"     global_real_argb_salmon        0x44     passive     all nine
 *   [7]  search    "search"    global_real_argb_aqua          0x2C     pursuit     begin perform update control flush_pos
 *   [8]  wait      "wait"      global_real_argb_magenta       0x18     pursuit     begin perform update control
 *   [9]  vehicle   "vehicle"   global_real_argb_darkgreen     0x4C     transitory  begin perform update control
 *   [10] charge    "charge"    global_real_argb_red           0x38     active      begin perform update control
 *   [11] obey      "obey"      global_real_argb_purple        0x84     transitory  begin perform update control end
 *   [12] converse  "converse"  global_real_argb_orange        0x14     transitory  begin perform update control end replace_prop
 *   [13] avoid     "avoid"     global_real_argb_grey          0x04     transitory  begin perform update control end
 *
 * The name pointers are the .rdata literals at 0x8200466C "none", 0x8211877C "sleep",
 * 0x820DC020 "alert", 0x821286BC "fight", 0x82113328 "flee", 0x821286B4 "uncover",
 * 0x821286AC "guard", 0x821286A4 "search", 0x8207BC94 "wait", 0x8211462C "vehicle",
 * 0x8212869C "charge", 0x82128694 "obey", 0x82128688 "converse", 0x82128680 "avoid".
 *
 * The object is in .rdata; the corpus declaration (src/headers/action_specification.h, plus four
 * consumer TUs) is non-const and the definition matches it so every declaration agrees.
 */

#include <stdint.h>
#include "../headers/real_argb_color.h"
#include "../headers/action_specification.h"
#include "../headers/actor_action.h"
#include "../headers/action_class.h"

/* Debug colors: pointers-to-color in .data, and the color slot holds their ADDRESS
 * (actor_action_debug_color.c dereferences it twice). Spelled exactly as in
 * src/headers/blam_data_globals.h. */
extern const real_argb_color *global_real_argb_black;      /* 0x84176D64 */
extern const real_argb_color *global_real_argb_lightblue;  /* 0x84176D84 */
extern const real_argb_color *global_real_argb_white;      /* 0x84176D5C */
extern const real_argb_color *global_real_argb_yellow;     /* 0x84176D78 */
extern const real_argb_color *global_real_argb_blue;       /* 0x84176D70 */
extern const real_argb_color *global_real_argb_salmon;     /* 0x84176D98 */
extern const real_argb_color *global_real_argb_darkgreen;  /* 0x84176D94 */
extern const real_argb_color *global_real_argb_red;        /* 0x84176D68 */
extern const real_argb_color *global_real_argb_purple;     /* 0x84176D8C */
extern const real_argb_color *global_real_argb_orange;     /* 0x84176D88 */
/* These four have no definition anywhere in the corpus yet (they are .data storage gaps of the
 * same family as the thirteen that do); declared here at the canonical type so this table can
 * take their address. */
extern const real_argb_color *global_real_argb_cyan;       /* 0x84176D74 */
extern const real_argb_color *global_real_argb_aqua;       /* 0x84176D90 */
extern const real_argb_color *global_real_argb_magenta;    /* 0x84176D7C */
extern const real_argb_color *global_real_argb_grey;       /* 0x84176D60 */

/* Every callback below is declared with the funcptr-SLOT signature taken from the matching
 * action_specification member, NOT the concrete signature of its definition TU: the definitions
 * spell the actor index variously `int` and `uint16_t`, which mismatches the slot and would raise
 * C4113. They exist only to have their address taken for this dispatch table. */
extern void    action_sleep_control(int actor_index);                       /* 0x83826DD8 */

extern void    action_alert_begin(int actor_index);                         /* 0x83826550 */
extern uint8_t action_alert_perform(int actor_index);                       /* 0x83826B20 */
extern void    action_alert_update(int actor_index);                        /* 0x83826558 */
extern void    action_alert_control(int actor_index);                       /* 0x838266D0 */
extern void    action_alert_flush_position_indices(int actor_index);        /* 0x83826728 */
extern void    action_alert_flush_structure_indices(int actor_index);       /* 0x83826758 */

extern void    action_fight_begin(int actor_index);                         /* 0x83825FA8 */
extern uint8_t action_fight_perform(int actor_index);                       /* 0x83825FB8 */
extern void    action_fight_update(int actor_index);                        /* 0x838263E0 */
extern void    action_fight_control(int actor_index);                       /* 0x83826450 */
extern void    action_fight_end(int actor_index);                           /* 0x83825FB0 */

extern void    action_flee_begin(int actor_index);                          /* 0x83825898 */
extern uint8_t action_flee_perform(int actor_index);                        /* 0x83825920 */
extern void    action_flee_update(int actor_index);                         /* 0x83825CE0 */
extern void    action_flee_control(int actor_index);                        /* 0x83825DC8 */
extern void    action_flee_end(int actor_index);                            /* 0x83825130 */
extern void    action_flee_modify_color(int actor_index, real_argb_color *color);                      /* 0x83825190 */
extern void    action_flee_replace_prop(int actor_index, int invalid_prop_index, int replacement_prop_index); /* 0x83825708 */
extern void    action_flee_flush_position_indices(int actor_index);         /* 0x83825160 */

extern void    action_uncover_begin(int actor_index);                       /* 0x83824938 */
extern uint8_t action_uncover_perform(int actor_index);                     /* 0x83824A60 */
extern void    action_uncover_update(int actor_index);                      /* 0x83824BD8 */
extern void    action_uncover_control(int actor_index);                     /* 0x83824EB0 */
extern void    action_uncover_modify_color(int actor_index, real_argb_color *color);                   /* 0x838250D0 */
extern void    action_uncover_flush_position_indices(int actor_index);      /* 0x83825068 */

extern void    action_guard_begin(int actor_index);                         /* 0x83823BD8 */
extern uint8_t action_guard_perform(int actor_index);                       /* 0x83823DE8 */
extern void    action_guard_update(int actor_index);                        /* 0x83823FE0 */
extern void    action_guard_control(int actor_index);                       /* 0x83824208 */
extern void    action_guard_end(int actor_index);                           /* 0x83823C40 */
extern void    action_guard_modify_color(int actor_index, real_argb_color *color);                     /* 0x838246E8 */
extern void    action_guard_replace_prop(int actor_index, int invalid_prop_index, int replacement_prop_index); /* 0x83824778 */
extern void    action_guard_flush_position_indices(int actor_index);        /* 0x83824620 */
extern void    action_guard_flush_structure_indices(int actor_index);       /* 0x838246B8 */

extern void    action_search_begin(int actor_index);                        /* 0x83822D20 */
extern uint8_t action_search_perform(int actor_index);                      /* 0x83822DD8 */
extern void    action_search_update(int actor_index);                       /* 0x83823208 */
extern void    action_search_control(int actor_index);                      /* 0x83823450 */
extern void    action_search_flush_position_indices(int actor_index);       /* 0x838233E8 */

extern void    action_wait_begin(int actor_index);                          /* 0x838227C0 */
extern uint8_t action_wait_perform(int actor_index);                        /* 0x838227C8 */
extern void    action_wait_update(int actor_index);                         /* 0x83822978 */
extern void    action_wait_control(int actor_index);                        /* 0x83822AA8 */

extern void    action_vehicle_begin(int actor_index);                       /* 0x838213F8 */
extern uint8_t action_vehicle_perform(int actor_index);                     /* 0x838220C8 */
extern void    action_vehicle_update(int actor_index);                      /* 0x83821460 */
extern void    action_vehicle_control(int actor_index);                     /* 0x83821490 */

extern void    action_charge_begin(int actor_index);                        /* 0x837F9010 */
extern uint8_t action_charge_perform(int actor_index);                      /* 0x837F98C8 */
extern void    action_charge_update(int actor_index);                       /* 0x837F9288 */
extern void    action_charge_control(int actor_index);                      /* 0x837F92E0 */

extern void    action_obey_begin(int actor_index);                          /* 0x837DF218 */
extern uint8_t action_obey_perform(int actor_index);                        /* 0x837DF2A8 */
extern void    action_obey_update(int actor_index);                         /* 0x837DF278 */
extern void    action_obey_control(int actor_index);                        /* 0x837DF3C8 */
extern void    action_obey_end(int actor_index);                            /* 0x837DF248 */

extern void    action_converse_begin(int actor_index);                      /* 0x837FCB08 */
extern uint8_t action_converse_perform(int actor_index);                    /* 0x837FCB10 */
extern void    action_converse_update(int actor_index);                     /* 0x837FCC38 */
extern void    action_converse_control(int actor_index);                    /* 0x837FCC40 */
extern void    action_converse_end(int actor_index);                        /* 0x837FCE40 */
extern void    action_converse_replace_prop(int actor_index, int invalid_prop_index, int replacement_prop_index); /* 0x837FCCF8 */

extern void    action_avoid_begin(int actor_index);                         /* 0x83821288 */
extern uint8_t action_avoid_perform(int actor_index);                       /* 0x83821298 */
extern void    action_avoid_update(int actor_index);                        /* 0x83821358 */
extern void    action_avoid_control(int actor_index);                       /* 0x83821360 */
extern void    action_avoid_end(int actor_index);                           /* 0x83821290 */

action_specification global_action_functions[14] =
{
    {   /* +0x000 */
        actor_action_none, "none", &global_real_argb_black, 0x00,
        _action_class_noncombat, { 0 },
        nullptr, nullptr, nullptr, nullptr, nullptr,
        nullptr, nullptr, nullptr, nullptr
    },
    {   /* +0x038 */
        actor_action_sleep, "sleep", &global_real_argb_lightblue, 0x00,
        _action_class_noncombat, { 0 },
        nullptr, nullptr, nullptr, action_sleep_control, nullptr,
        nullptr, nullptr, nullptr, nullptr
    },
    {   /* +0x070 */
        actor_action_alert, "alert", &global_real_argb_cyan, 0x5C,
        _action_class_noncombat, { 0 },
        action_alert_begin, action_alert_perform, action_alert_update, action_alert_control, nullptr,
        nullptr, nullptr, action_alert_flush_position_indices, action_alert_flush_structure_indices
    },
    {   /* +0x0A8 */
        actor_action_fight, "fight", &global_real_argb_white, 0x04,
        _action_class_active, { 0 },
        action_fight_begin, action_fight_perform, action_fight_update, action_fight_control, action_fight_end,
        nullptr, nullptr, nullptr, nullptr
    },
    {   /* +0x0E0 */
        actor_action_flee, "flee", &global_real_argb_yellow, 0x30,
        _action_class_transitory, { 0 },
        action_flee_begin, action_flee_perform, action_flee_update, action_flee_control, action_flee_end,
        action_flee_modify_color, action_flee_replace_prop, action_flee_flush_position_indices, nullptr
    },
    {   /* +0x118 */
        actor_action_uncover, "uncover", &global_real_argb_blue, 0x34,
        _action_class_pursuit, { 0 },
        action_uncover_begin, action_uncover_perform, action_uncover_update, action_uncover_control, nullptr,
        action_uncover_modify_color, nullptr, action_uncover_flush_position_indices, nullptr
    },
    {   /* +0x150 */
        actor_action_guard, "guard", &global_real_argb_salmon, 0x44,
        _action_class_passive, { 0 },
        action_guard_begin, action_guard_perform, action_guard_update, action_guard_control, action_guard_end,
        action_guard_modify_color, action_guard_replace_prop, action_guard_flush_position_indices, action_guard_flush_structure_indices
    },
    {   /* +0x188 */
        actor_action_search, "search", &global_real_argb_aqua, 0x2C,
        _action_class_pursuit, { 0 },
        action_search_begin, action_search_perform, action_search_update, action_search_control, nullptr,
        nullptr, nullptr, action_search_flush_position_indices, nullptr
    },
    {   /* +0x1C0 */
        actor_action_wait, "wait", &global_real_argb_magenta, 0x18,
        _action_class_pursuit, { 0 },
        action_wait_begin, action_wait_perform, action_wait_update, action_wait_control, nullptr,
        nullptr, nullptr, nullptr, nullptr
    },
    {   /* +0x1F8 */
        actor_action_vehicle, "vehicle", &global_real_argb_darkgreen, 0x4C,
        _action_class_transitory, { 0 },
        action_vehicle_begin, action_vehicle_perform, action_vehicle_update, action_vehicle_control, nullptr,
        nullptr, nullptr, nullptr, nullptr
    },
    {   /* +0x230 */
        actor_action_charge, "charge", &global_real_argb_red, 0x38,
        _action_class_active, { 0 },
        action_charge_begin, action_charge_perform, action_charge_update, action_charge_control, nullptr,
        nullptr, nullptr, nullptr, nullptr
    },
    {   /* +0x268 */
        actor_action_obey, "obey", &global_real_argb_purple, 0x84,
        _action_class_transitory, { 0 },
        action_obey_begin, action_obey_perform, action_obey_update, action_obey_control, action_obey_end,
        nullptr, nullptr, nullptr, nullptr
    },
    {   /* +0x2A0 */
        actor_action_converse, "converse", &global_real_argb_orange, 0x14,
        _action_class_transitory, { 0 },
        action_converse_begin, action_converse_perform, action_converse_update, action_converse_control, action_converse_end,
        nullptr, action_converse_replace_prop, nullptr, nullptr
    },
    {   /* +0x2D8 */
        actor_action_avoid, "avoid", &global_real_argb_grey, 0x04,
        _action_class_transitory, { 0 },
        action_avoid_begin, action_avoid_perform, action_avoid_update, action_avoid_control, action_avoid_end,
        nullptr, nullptr, nullptr, nullptr
    },
};
