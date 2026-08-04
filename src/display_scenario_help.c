#include <stdint.h>
/* display_scenario_help @0x83736C20 — show a scenario hint string on the HUD by firing the HCEX host event
 * "uiHaloShowHint" with the string index. */

extern void hcex_fire_event_i(const char *event_name, const char *param_name, int value);

void display_scenario_help(int16_t string_index)
{
    hcex_fire_event_i("uiHaloShowHint", "index", string_index);
}
