/* hcex_reset_weapon_state @0x836839E8 — clear the per-local-player shadow copies of the three classic
 * Halo CE weapon HUD values so the next hcex_update_weapon_state pushes fresh state. Ammo is reset to -1
 * (no cached value); the compass headings and plasma heat levels are zeroed. Two slots each = split-screen
 * local players 0 and 1. */

extern "C" int   hcex_arifle_ammo[];
extern "C" float hcex_arifle_compass[];
extern "C" float hcex_plasma_rifle_heat[];
extern "C" float hcex_plasma_pistol_heat[];

extern "C" void hcex_reset_weapon_state(void)
{
    hcex_arifle_ammo[0] = -1;
    hcex_arifle_ammo[1] = -1;
    hcex_arifle_compass[0] = 0.0f;
    hcex_arifle_compass[1] = 0.0f;
    hcex_plasma_rifle_heat[0] = 0.0f;
    hcex_plasma_rifle_heat[1] = 0.0f;
    hcex_plasma_pistol_heat[0] = 0.0f;
    hcex_plasma_pistol_heat[1] = 0.0f;
}
