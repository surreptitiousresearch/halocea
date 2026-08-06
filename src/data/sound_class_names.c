/* sound_class_names @ 0x8417D330 (.data, 204 bytes)
 * DB applied_types: const char *sound_class_names[51];
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x8211D4C8 -> "projectile_impact"
 *   +0x0004 = 0x8211D4B0 -> "projectile_detonation"
 *   +0x0008 = 0x8200155A -> "" (empty string at target)
 *   +0x000C = 0x8200155A -> "" (empty string at target)
 *   +0x0010 = 0x82114C20 -> "weapon_fire"
 *   +0x0014 = 0x8211D4A0 -> "weapon_ready"
 *   +0x0018 = 0x8211D490 -> "weapon_reload"
 *   +0x001C = 0x8211D480 -> "weapon_empty"
 *   +0x0020 = 0x8211D470 -> "weapon_charge"
 *   +0x0024 = 0x8211D460 -> "weapon_overheat"
 *   +0x0028 = 0x8211D454 -> "weapon_idle"
 *   +0x002C = 0x8200155A -> "" (empty string at target)
 *   +0x0030 = 0x8200155A -> "" (empty string at target)
 *   +0x0034 = 0x8211D444 -> "object_impacts"
 *   +0x0038 = 0x8211D430 -> "particle_impacts"
 *   +0x003C = 0x8211D418 -> "slow_particle_impacts"
 *   +0x0040 = 0x8200155A -> "" (empty string at target)
 *   +0x0044 = 0x8200155A -> "" (empty string at target)
 *   +0x0048 = 0x8211D408 -> "unit_footsteps"
 *   +0x004C = 0x8211D3FC -> "unit_dialog"
 *   +0x0050 = 0x8200155A -> "" (empty string at target)
 *   +0x0054 = 0x8200155A -> "" (empty string at target)
 *   +0x0058 = 0x8211D3E8 -> "vehicle_collision"
 *   +0x005C = 0x8211D3D8 -> "vehicle_engine"
 *   +0x0060 = 0x8200155A -> "" (empty string at target)
 *   +0x0064 = 0x8200155A -> "" (empty string at target)
 *   +0x0068 = 0x8211D3CC -> "device_door"
 *   +0x006C = 0x8211D3B8 -> "device_force_field"
 *   +0x0070 = 0x8211D3A4 -> "device_machinery"
 *   +0x0074 = 0x8211D394 -> "device_nature"
 *   +0x0078 = 0x8211D380 -> "device_computers"
 *   +0x007C = 0x8200155A -> "" (empty string at target)
 *   +0x0080 = 0x8204836C -> "music"
 *   +0x0084 = 0x8211D370 -> "ambient_nature"
 *   +0x0088 = 0x8211D35C -> "ambient_machinery"
 *   +0x008C = 0x8211D348 -> "ambient_computers"
 *   +0x0090 = 0x8200155A -> "" (empty string at target)
 *   +0x0094 = 0x8200155A -> "" (empty string at target)
 *   +0x0098 = 0x8200155A -> "" (empty string at target)
 *   +0x009C = 0x8211D334 -> "first_person_damage"
 *   ... 44 further bytes elided; full hex in .sweep/data_image.tsv
 * sound_class_names (0x8417D330). const char*[51]; empty slots are "" (0x8200155A).
 */
const char *sound_class_names[51] = {
    "projectile_impact",
    "projectile_detonation",
    "",
    "",
    "weapon_fire",
    "weapon_ready",
    "weapon_reload",
    "weapon_empty",
    "weapon_charge",
    "weapon_overheat",
    "weapon_idle",
    "",
    "",
    "object_impacts",
    "particle_impacts",
    "slow_particle_impacts",
    "",
    "",
    "unit_footsteps",
    "unit_dialog",
    "",
    "",
    "vehicle_collision",
    "vehicle_engine",
    "",
    "",
    "device_door",
    "device_force_field",
    "device_machinery",
    "device_nature",
    "device_computers",
    "",
    "music",
    "ambient_nature",
    "ambient_machinery",
    "ambient_computers",
    "",
    "",
    "",
    "first_person_damage",
    "",
    "",
    "",
    "",
    "scripted_dialog_player",
    "scripted_effect",
    "scripted_dialog_other",
    "scripted_dialog_force_unspatialized",
    "",
    "",
    "game_event",
};
