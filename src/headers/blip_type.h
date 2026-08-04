#pragma once
/* Motion-sensor / HUD blip classification enums.
 * blip_type — anon DB enum _B3D2ADF248242B5939540C1C55B9E9EE (blip_type_get result / motion_sensor_blip.type).
 * hud_blip_type — anon DB enum _64BD189188FDBA53A166351EB417778F (motion_sensor_blip.size). */

enum blip_type
{
    _blip_type_self = 0,
    _blip_type_friend = 1,
    _blip_type_enemy = 2,
    _blip_type_vehicle_friend = 3,
    _blip_type_vehicle_enemy = 4,
    _blip_type_custom = 5,
    _blip_type_none = 6,
    NUMBER_OF_BLIP_TYPES = 7,
};

enum hud_blip_type
{
    _hud_blip_type_medium = 0,
    _hud_blip_type_small = 1,
    _hud_blip_type_large = 2,
    NUMBER_OF_HUD_BLIP_TYPES = 3,
};
