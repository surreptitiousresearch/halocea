/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* set_effect_ambient @0x836A6BD0 — compute the global ambient screen-effect tint (a grey level, e.g. the
 * active-camouflage/flashlight HUD overlay) for this frame. Defaults to black; when in gameplay with a single
 * local window and the weapon HUD requests the effect, sets all three channels to
 * min(0.75, flashlight_power_clamped * scale * screen_effect_value_clamped). */

#include <stdint.h>
#include "headers/render_globals.h"
#include "headers/real_rgb_color.h"
#include "headers/weapon_hud_interface_definition.h"
#include "headers/hud_screen_effect_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/hud_screen_effect_mask_only_when_zoomed_flags.h"
#include "headers/hud_screen_effect_desaturation_flags.h"
#include "headers/blam_data_globals.h"

extern int interface_get_weapon_hud_index(float *flashlight_power);
extern int16_t player_control_get_zoom_level(int16_t local_player_index);
extern int16_t main_get_window_count(void);
extern float rasterizer_script_screen_effect_get_value(int16_t index);

void set_effect_ambient(uint8_t in_game_play)
{
    global_effect_ambient.__s1.red = 0.0f;
    global_effect_ambient.__s1.green = 0.0f;
    global_effect_ambient.__s1.blue = 0.0f;

    if (!in_game_play)
        return;
    if ((uint16_t)render.local_player_index == 0xFFFF)
        return;

    float flashlight_power;
    int weapon_hud_index = interface_get_weapon_hud_index(&flashlight_power);
    if (weapon_hud_index == -1)
        return;

    weapon_hud_interface_definition *hud_definition = TAG_GET(weapon_hud_interface_definition, weapon_hud_index);
    if (hud_definition->screen_effects.count <= 0)
        return;
    hud_screen_effect_definition *element = (hud_screen_effect_definition *)hud_definition->screen_effects.address;

    if ((player_control_get_zoom_level(render.local_player_index) != -1 || (element->mask_flags & (1u << _hud_screen_effect_mask_only_when_zoomed_bit)) == 0)
        && main_get_window_count() <= 1
        && element->mask_fullscreen.index != -1
        && (element->desaturation_flags & (1u << _hud_screen_effect_desaturation_connect_to_flashlight_bit)) != 0)
    {
        float power = flashlight_power;
        if (power < 0.0f)
            power = 0.0f;
        else if (power > 1.0f)
            power = 1.0f;
        float scaled = power * element->desaturation_intensity;

        /* Decompiler calls the getter three times for the clamp/use; called once here (same index, pure). */
        float effect_value = rasterizer_script_screen_effect_get_value(element->desaturation_script_source);
        if (effect_value < 0.0f)
            effect_value = 0.0f;
        else if (effect_value > 1.0f)
            effect_value = 1.0f;

        float ambient = effect_value * scaled;
        if (ambient > 0.0f)
        {
            float clamped = ambient >= 0.75f ? 0.75f : ambient;
            global_effect_ambient.__s1.red = clamped;
            global_effect_ambient.__s1.green = clamped;
            global_effect_ambient.__s1.blue = clamped;
        }
    }
}
