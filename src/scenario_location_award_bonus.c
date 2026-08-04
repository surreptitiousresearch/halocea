/* scenario_location_award_bonus @0x83703478 — clears a location's pickup-bonus marker (sets bonus = -1). */

#include "headers/location.h"

void scenario_location_award_bonus(location *location)
{
    location->bonus = -1;
}
