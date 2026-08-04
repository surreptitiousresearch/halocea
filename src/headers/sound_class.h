#pragma once
/* sound_class — Blam sound-class enumeration (indexes sound_class_names[] / sound_classes[]).
 * DB anonymous enum _3ECE4FFB551BA4B4D7DAFD8C876ECB34 (verbatim). */
#ifndef SOUND_CLASS_H
#define SOUND_CLASS_H

enum sound_class
{
    _sound_class_projectile_impact = 0,
    _sound_class_projectile_detonation = 1,
    _sound_class_projectile_unused0 = 2,
    _sound_class_projectile_unused1 = 3,
    _sound_class_weapon_fire = 4,
    _sound_class_weapon_ready = 5,
    _sound_class_weapon_reload = 6,
    _sound_class_weapon_empty = 7,
    _sound_class_weapon_charge = 8,
    _sound_class_weapon_overheat = 9,
    _sound_class_weapon_idle = 10,
    _sound_class_weapon_unused0 = 11,
    _sound_class_weapon_unused1 = 12,
    _sound_class_object_impacts = 13,
    _sound_class_particle_impacts = 14,
    _sound_class_slow_impacts = 15,
    _sound_class_effect_unused2 = 16,
    _sound_class_effect_unused3 = 17,
    _sound_class_footstep = 18,
    _sound_class_unit_dialog = 19,
    _sound_class_unit_unused0 = 20,
    _sound_class_unit_unused1 = 21,
    _sound_class_vehicle_impact = 22,
    _sound_class_vehicle_engine = 23,
    _sound_class_vehicle_unused0 = 24,
    _sound_class_vehicle_unused1 = 25,
    _sound_class_device_door = 26,
    _sound_class_device_force_field = 27,
    _sound_class_device_machinery = 28,
    _sound_class_device_nature = 29,
    _sound_class_device_computers = 30,
    _sound_class_device_unused1 = 31,
    _sound_class_music = 32,
    _sound_class_ambient_nature = 33,
    _sound_class_ambient_machinery = 34,
    _sound_class_ambient_computers = 35,
    _sound_class_marty_unused1 = 36,
    _sound_class_marty_unused2 = 37,
    _sound_class_marty_unused3 = 38,
    _sound_class_player_hurt = 39,
    _sound_class_player_unused0 = 40,
    _sound_class_player_unused1 = 41,
    _sound_class_player_unused2 = 42,
    _sound_class_player_unused3 = 43,
    _sound_class_scripted_dialog_to_player = 44,
    _sound_class_scripted_other = 45,
    _sound_class_scripted_dialog_to_other = 46,
    _sound_class_scripted_dialog_force_unspatialized = 47,
    _sound_class_scripted_unused2 = 48,
    _sound_class_scripted_unused3 = 49,
    _sound_class_game_event = 50,
    NUMBER_OF_SOUND_CLASSES = 51
};

#endif /* SOUND_CLASS_H */
