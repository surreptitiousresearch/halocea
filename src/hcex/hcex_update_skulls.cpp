/* hcex_update_skulls @0x823D4FC8 — per-frame(?) HCEX bridge refresh of every classic Halo skull's
 * effective on/off state (persisted config value OR the live debug-console toggle,
 * via hcex_get_skull_state), driving the assorted gameplay-modifier globals the Blam engine
 * reads elsewhere. Skull index -> effect map (as wired here):
 *   1  Iron           -> hcex_reset_single_on_die
 *   2  Black Eye      -> hcex_disable_motion_sensor
 *   3  Assassins      -> hcex_mythic_skull
 *   4  Famine         -> hcex_ai_drop_wpn_half_ammo
 *   5  Thunderstorm    -> hcex_set_explode_grunts(state) (tag-side toggle, not a plain global)
 *   6  Tough Luck     -> hcex_allow_covenant_weapon (inverted: skull ON disallows)
 *   7  Catch          -> player_magnetism_flag / hcex_off_aim_projectile (both derived from one
 *                        cntlzw "== 0" boolean test on the same skull-state read)
 *   8  Fog            -> hcex_2x_rounds_per_shot
 *   9  Nightmare      -> hcex_malfunction_skull (also resets hcex_off_hud_element when OFF)
 *   10 (bandana-ish)  -> hcex_recharge_on_melee
 *   11 (GBP)          -> hcex_skull_GruntBirthdayParty
 *   12               -> hcex_unit_drop_grenade_on_melee
 *   13               -> hcex_mouth_skull
 *   14               -> hcex_infinite_ammo
 *   15               -> hcex_double_damage_radius
 * `(_cntlzw(x) & 0x20) != 0` is the codebase's established "x == 0" boolean idiom. */

#include "../headers/hcex/hcex_dbgvar_boundary.h"   /* dbgVAR_bool (reused for dbg_skull01..15) */

extern int hcex_get_skull_state(int skull_index, bool dbg_var);
extern void hcex_set_explode_grunts(int val);

extern int hcex_gold_skulls_count;
extern int hcex_silver_skulls_count;
extern int hcex_reset_single_on_die;
extern int hcex_disable_motion_sensor;
extern int hcex_mythic_skull;
extern int hcex_ai_drop_wpn_half_ammo;
extern int hcex_allow_covenant_weapon;
extern unsigned char player_magnetism_flag;
extern int hcex_off_aim_projectile;
extern int hcex_2x_rounds_per_shot;
extern int hcex_malfunction_skull;
extern int hcex_off_hud_element;
extern int hcex_recharge_on_melee;
extern int hcex_skull_GruntBirthdayParty;
extern int hcex_unit_drop_grenade_on_melee;
extern float hcex_aim_assist_coeff;
extern int hcex_mouth_skull;
extern int hcex_infinite_ammo;
extern int hcex_double_damage_radius;

extern dbgVAR_bool dbg_skull01, dbg_skull02, dbg_skull03, dbg_skull04, dbg_skull05,
                    dbg_skull06, dbg_skull07, dbg_skull08, dbg_skull09, dbg_skull10,
                    dbg_skull11, dbg_skull12, dbg_skull13, dbg_skull14, dbg_skull15;

void hcex_update_skulls(void)
{
    hcex_gold_skulls_count = 0;
    hcex_silver_skulls_count = 0;

    hcex_reset_single_on_die     = hcex_get_skull_state(1, dbg_skull01.value);
    hcex_disable_motion_sensor   = hcex_get_skull_state(2, dbg_skull02.value);
    hcex_mythic_skull            = hcex_get_skull_state(3, dbg_skull03.value);
    hcex_ai_drop_wpn_half_ammo   = hcex_get_skull_state(4, dbg_skull04.value);

    int thunderstorm = hcex_get_skull_state(5, dbg_skull05.value);
    hcex_set_explode_grunts(thunderstorm);

    hcex_allow_covenant_weapon = hcex_get_skull_state(6, dbg_skull06.value) == 0;

    int catch_skull = hcex_get_skull_state(7, dbg_skull07.value);
    player_magnetism_flag  = (catch_skull == 0);   /* (_cntlzw(x) & 0x20) != 0        ==  x == 0 */
    hcex_off_aim_projectile = (catch_skull != 0);  /* ((_cntlzw(x) >> 5) & 1) == 0    ==  x != 0 */

    hcex_2x_rounds_per_shot = hcex_get_skull_state(8, dbg_skull08.value);

    hcex_malfunction_skull = hcex_get_skull_state(9, dbg_skull09.value);
    if ( !hcex_malfunction_skull )
        hcex_off_hud_element = -1;

    hcex_recharge_on_melee         = hcex_get_skull_state(10, dbg_skull10.value);
    hcex_skull_GruntBirthdayParty  = hcex_get_skull_state(11, dbg_skull11.value);
    hcex_unit_drop_grenade_on_melee = hcex_get_skull_state(12, dbg_skull12.value);

    hcex_aim_assist_coeff = 1.0f;

    hcex_mouth_skull          = hcex_get_skull_state(13, dbg_skull13.value);
    hcex_infinite_ammo        = hcex_get_skull_state(14, dbg_skull14.value);
    hcex_double_damage_radius = hcex_get_skull_state(15, dbg_skull15.value);
}
