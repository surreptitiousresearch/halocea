/* initialize_hud_state @0x836D65E8 — reset a unit's HUD state to its "nothing cached yet" defaults so the
 * next update forces a full refresh of health/shield/motion-sensor readouts. */

#include "headers/unit_hud_state.h"

void initialize_hud_state(unit_hud_state *hud_state)
{
    hud_state->auxilary_flash_time[0] = -1;
    hud_state->last_health_flash_time = -1;
    hud_state->last_motion_sensor_flash_time = -1;
    hud_state->last_body_vitality = -1.0f;
    hud_state->last_unit_index = -1;
    hud_state->last_shield_vitality = -1.0f;
    hud_state->fade_time = -1.0f;
}
