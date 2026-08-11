/* hud_dispose_from_old_map @0x83707E60 — per-map HUD teardown: dispose each HUD subsystem's
 * old-map state in turn.
 *
 * DEVIATION: the decompiler threads r3 between the five calls as if chained; they are independent
 * no-argument disposes. */

extern void motion_sensor_dispose_from_old_map(void);
extern void hud_dispose_nav_points_from_old_map(void);
extern void hud_dispose_weapon_interface_from_old_map(void);
extern void hud_dispose_unit_interface_from_old_map(void);
extern void hud_messaging_dispose_from_old_map(void);

void hud_dispose_from_old_map(void)
{
    motion_sensor_dispose_from_old_map();
    hud_dispose_nav_points_from_old_map();
    hud_dispose_weapon_interface_from_old_map();
    hud_dispose_unit_interface_from_old_map();
    hud_messaging_dispose_from_old_map();
}
