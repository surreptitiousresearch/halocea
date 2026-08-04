/* hud_initialize_for_new_map @0x83707DD0 — bring up the HUD for a new map: reset scripted HUD visibility,
 * resolve the HUD globals tag, then initialize the messaging, unit, weapon, nav-point and motion-sensor
 * interfaces in turn. */

#include <stdint.h>
#include "headers/hud_scripted_globals.h"
#include "headers/hud_globals.h"
#include "headers/global_tag_instances.h"
#include "headers/interface_tag_index.h"

extern int interface_get_tag_index(int16_t interface_tag_index);
extern void hud_messaging_initialize_for_new_map(void);
extern void hud_initialize_unit_interface_for_new_map(void);
extern void hud_initialize_weapon_interface_for_new_map(void);
extern void hud_initialize_nav_points_for_new_map(void);
extern void motion_sensor_initialize_for_new_map(void);

void hud_initialize_for_new_map(void)
{
    hud_scripted_globals->show_hud = 0;
    hud_scripted_globals->show_hud_help_text = 0;
    hud_scripted_globals->pad[0] = 0;
    hud_scripted_globals->pad[1] = 0;
    hud_scripted_globals->show_hud = 1;

    hud_globals = TAG_GET(hud_globals_definition, interface_get_tag_index(_interface_hud_globals));

    hud_messaging_initialize_for_new_map();
    hud_initialize_unit_interface_for_new_map();
    hud_initialize_weapon_interface_for_new_map();
    hud_initialize_nav_points_for_new_map();
    motion_sensor_initialize_for_new_map(); /* attested void: r3-thread return removed */
}
