/* hud_dispose @0x83707DAC — one-time teardown of the HUD: tears down the motion sensor, nav points, weapon
 * interface, unit interface, and messaging in order.
 * DEVIATION: the decompiler threaded r3 through these as if each consumed the previous return value; they
 * are independent no-argument dispose calls. */

extern void motion_sensor_dispose(void);
extern void hud_dispose_nav_points(void);
extern void hud_dispose_weapon_interface(void);
extern void hud_dispose_unit_interface(void);
extern void hud_messaging_dispose(void);

void hud_dispose(void)
{
    motion_sensor_dispose();
    hud_dispose_nav_points();
    hud_dispose_weapon_interface();
    hud_dispose_unit_interface();
    hud_messaging_dispose();
}
