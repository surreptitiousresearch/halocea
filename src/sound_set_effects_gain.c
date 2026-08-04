/* sound_set_effects_gain @0x83? — set the global sound-effects gain. When the gain crosses the mute boundary
 * the full set of effect sound classes is toggled to match: going from silent (0) to audible re-enables them
 * all (and clamps the gain to 1.0), while dropping a positive gain to <= 0 disables them all and forces the gain
 * to exactly 0. Gain changes that do not cross the boundary just store the new value. Any change re-runs the
 * channel update.
 *
 * NOTE: the disable list spells one class "sound_class_ambient_computer" while the enable list uses
 * "sound_class_ambient_computers" — preserved verbatim from the original (two distinct string literals). */

#include <stdint.h>
#include "headers/sound_manager_globals.h"

extern void debug_sound_classes_enable(const char *substring, uint8_t enabled);
extern void update_channels(void);

void sound_set_effects_gain(float gain)
{
    float effects_gain = sound_manager_globals.effects_gain;
    if (gain == effects_gain)
        return;

    if (effects_gain <= 0.0f || gain > 0.0f)
    {
        if (effects_gain == 0.0f && gain > 0.0f)
        {
            debug_sound_classes_enable("sound_class_projectile_impact", 1);
            debug_sound_classes_enable("sound_class_projectile_detonation", 1);
            debug_sound_classes_enable("weapon_fire", 1);
            debug_sound_classes_enable("sound_class_weapon_ready", 1);
            debug_sound_classes_enable("sound_class_weapon_reload", 1);
            debug_sound_classes_enable("sound_class_weapon_empty", 1);
            debug_sound_classes_enable("sound_class_weapon_charge", 1);
            debug_sound_classes_enable("sound_class_weapon_overheat", 1);
            debug_sound_classes_enable("sound_class_weapon_idle", 1);
            debug_sound_classes_enable("sound_class_object_impacts", 1);
            debug_sound_classes_enable("sound_class_particle_impacts", 1);
            debug_sound_classes_enable("sound_class_slow_impacts", 1);
            debug_sound_classes_enable("sound_class_footstep", 1);
            debug_sound_classes_enable("sound_class_vehicle_impact", 1);
            debug_sound_classes_enable("sound_class_vehicle_engine", 1);
            debug_sound_classes_enable("sound_class_device_door", 1);
            debug_sound_classes_enable("sound_class_device_force_field", 1);
            debug_sound_classes_enable("sound_class_device_machinery", 1);
            debug_sound_classes_enable("sound_class_device_nature", 1);
            debug_sound_classes_enable("sound_class_device_computers", 1);
            debug_sound_classes_enable("sound_class_ambient_nature", 1);
            debug_sound_classes_enable("sound_class_ambient_machinery", 1);
            debug_sound_classes_enable("sound_class_ambient_computers", 1);
            debug_sound_classes_enable("sound_class_player_hurt", 1);
            debug_sound_classes_enable("sound_class_game_event", 1);
            sound_manager_globals.effects_gain = (gain >= 1.0f) ? 1.0f : gain;
        }
        else
        {
            sound_manager_globals.effects_gain = gain;
        }
    }
    else
    {
        debug_sound_classes_enable("sound_class_projectile_impact", 0);
        debug_sound_classes_enable("sound_class_projectile_detonation", 0);
        debug_sound_classes_enable("weapon_fire", 0);
        debug_sound_classes_enable("sound_class_weapon_ready", 0);
        debug_sound_classes_enable("sound_class_weapon_reload", 0);
        debug_sound_classes_enable("sound_class_weapon_empty", 0);
        debug_sound_classes_enable("sound_class_weapon_charge", 0);
        debug_sound_classes_enable("sound_class_weapon_overheat", 0);
        debug_sound_classes_enable("sound_class_weapon_idle", 0);
        debug_sound_classes_enable("sound_class_object_impacts", 0);
        debug_sound_classes_enable("sound_class_particle_impacts", 0);
        debug_sound_classes_enable("sound_class_slow_impacts", 0);
        debug_sound_classes_enable("sound_class_footstep", 0);
        debug_sound_classes_enable("sound_class_vehicle_impact", 0);
        debug_sound_classes_enable("sound_class_vehicle_engine", 0);
        debug_sound_classes_enable("sound_class_device_door", 0);
        debug_sound_classes_enable("sound_class_device_force_field", 0);
        debug_sound_classes_enable("sound_class_device_machinery", 0);
        debug_sound_classes_enable("sound_class_device_nature", 0);
        debug_sound_classes_enable("sound_class_device_computers", 0);
        debug_sound_classes_enable("sound_class_ambient_nature", 0);
        debug_sound_classes_enable("sound_class_ambient_machinery", 0);
        debug_sound_classes_enable("sound_class_ambient_computer", 0);
        debug_sound_classes_enable("sound_class_player_hurt", 0);
        debug_sound_classes_enable("sound_class_game_event", 0);
        sound_manager_globals.effects_gain = 0.0f;
    }

    update_channels();
}
