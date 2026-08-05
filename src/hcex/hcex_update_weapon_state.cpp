/* hcex_update_weapon_state @0x83683A30 — HCEX debug/telemetry bridge: when the active first-person weapon is
 * one of the three classic Halo CE weapons (assault rifle, plasma rifle, plasma pistol), push its live state
 * (heat/age/overheat, ammo, compass heading, and — for the plasma pistol — trigger and incoming/outgoing
 * network arrays) into the HCEX debug overlay, and forward changed values to the HCEX front-end model so the
 * recreated HUD updates. Per-local-player shadow copies (hcex_*_heat / hcex_arifle_ammo / hcex_arifle_compass)
 * suppress redundant updates.
 *
 * DEVIATION 1: compass_magnitude is a float that shadows a GPR slot (FPR-shadow), pushing the final wpn
 * argument onto the stack; the decompiler invented ~20 phantom int args and renamed wpn to a29. Restored to
 * the database's 9-argument prototype; wpn is read as a float array for the plasma-pistol telemetry.
 * DEVIATION 2: the three weapon-name comparisons were emitted as inlined byte loops; restored to strcmp(). */

#include <stdint.h>
#include "../headers/weapon_definition.h"
#include "../headers/weapon_interface_state.h"

typedef struct weapon_datum weapon_datum;

extern float hcex_plasma_pistol_heat[];
extern float hcex_plasma_rifle_heat[];
extern int hcex_arifle_ammo[];
extern float hcex_arifle_compass[];

extern int strcmp(const char *a, const char *b);
extern float __fabs(float x);
extern void hcex_dbg_add_float(const char *name, float v);
extern void hcex_print_compass(int idx, float compass_magnitude);
extern float hcex_update_heat(int modelId, int16_t local_player_index, float heat_level);
extern int hcex_update_ammo(int modelId, int16_t local_player_index, int arifle_ammo);
extern float hcex_update_compass(int modelId, int16_t local_player_index, float compass_magnitude);

extern "C" void hcex_update_weapon_state(int16_t local_player_index, const weapon_definition *w,
                              const weapon_interface_state *weapon_state, const int16_t *state_flags,
                              const int16_t *overlay_flags, const int16_t *numbers, const float *numbers_real,
                              float compass_magnitude, const weapon_datum *wpn)
{
    if (!w)
        return;

    const char *name = w->object.model.name; /* typed (was raw +0x2C) */
    if (!name)
        return;

    int model_id = w->weapon.interface_definition.first_person_model.index; /* typed (was raw +0x468) */
    const float *wpn_floats = (const float *)wpn;

    if (strcmp(name, "weapons\\assault rifle\\assault rifle") == 0)
    {
        hcex_print_compass(local_player_index, compass_magnitude);

        int rounds_loaded = weapon_state->magazines[0].rounds_loaded;
        if (hcex_arifle_ammo[local_player_index] != rounds_loaded)
            hcex_arifle_ammo[local_player_index] = hcex_update_ammo(model_id, local_player_index, rounds_loaded);

        if (__fabs(hcex_arifle_compass[local_player_index] - compass_magnitude) > 0.001f)
            hcex_arifle_compass[local_player_index] = hcex_update_compass(model_id, local_player_index,
                                                                          compass_magnitude);
    }
    else if (strcmp(name, "weapons\\plasma rifle\\plasma rifle") == 0)
    {
        hcex_dbg_add_float("plasma rifle heat", weapon_state->heat);
        hcex_dbg_add_float("plasma rifle overheated", (float)weapon_state->overheated);

        if (__fabs(hcex_plasma_rifle_heat[local_player_index] - weapon_state->heat) > 0.01f)
            hcex_plasma_rifle_heat[local_player_index] = hcex_update_heat(model_id, local_player_index,
                                                                          weapon_state->heat);
    }
    else if (strcmp(name, "weapons\\plasma pistol\\plasma pistol") == 0)
    {
        hcex_dbg_add_float("plasma pistol primary trigger", wpn_floats[141]);
        hcex_dbg_add_float("plasma pistol age", weapon_state->age);
        hcex_dbg_add_float("plasma pistol heat", weapon_state->heat);
        hcex_dbg_add_float("plasma pistol overheated", (float)weapon_state->overheated);
        hcex_dbg_add_float("plasma pistol incoming 0", wpn_floats[73]);
        hcex_dbg_add_float("plasma pistol incoming 1", wpn_floats[74]);
        hcex_dbg_add_float("plasma pistol incoming 2", wpn_floats[75]);
        hcex_dbg_add_float("plasma pistol incoming 3", wpn_floats[76]);
        hcex_dbg_add_float("plasma pistol outgoing 0", wpn_floats[77]);
        hcex_dbg_add_float("plasma pistol outgoing 1", wpn_floats[78]);
        hcex_dbg_add_float("plasma pistol outgoing 2", wpn_floats[79]);
        hcex_dbg_add_float("plasma pistol outgoing 3", wpn_floats[80]);

        if (__fabs(hcex_plasma_pistol_heat[local_player_index] - wpn_floats[74]) > 0.01f)
            hcex_plasma_pistol_heat[local_player_index] = hcex_update_heat(model_id, local_player_index,
                                                                           wpn_floats[75]);
    }
}
