#include "headers/blam_data_globals.h"
/* hud_initialize @ 0x83707D50 — allocate HUD scripted globals and init HUD subsystems */

typedef struct hud_scripted_globals_definition hud_scripted_globals_definition;
extern void *game_state_malloc(const char *name, const char *type, int size);
extern void hud_messaging_initialize(void);
extern void hud_initialize_unit_interface(void);
extern void hud_initialize_weapon_interface(void);
extern void hud_initialize_nav_points(void);
extern void motion_sensor_initialize(void);

void hud_initialize(void)
{
    hud_scripted_globals = game_state_malloc("hud scripted globals", 0, 4);hud_messaging_initialize();hud_initialize_unit_interface();hud_initialize_weapon_interface();hud_initialize_nav_points();
    motion_sensor_initialize(); /* attested void: r3-thread + phantom arg dropped */
}
