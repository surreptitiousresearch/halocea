#include "headers/game_variant.h"
#include "headers/game_engine_vehicles.h"

extern game_variant *game_engine_get_variant(void);

int vehicle_remapper_get_vehicles_default_mode(int team, int vehicle_type)
{
    game_variant *variant = game_engine_get_variant();

    if ( !variant )
        return 0;

    vehicle_options_s value = (team == 1)
        ? variant->universal_variant.vehicle_set2
        : variant->universal_variant.vehicle_set;

    return value.__s1.preset == _game_engine_vehicles_default;
}
