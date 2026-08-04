#pragma once
/* allegiance_incident_type — the kind of hostility incident reported to
 * game_allegiance_incident. From DB anonymous enum _F7FAF4B404A6411445CF277811DD6143. */

enum allegiance_incident_type
{
    _allegiance_incident_accident = 0, /* minor: +1 incident weight */
    _allegiance_incident_betrayal = 1, /* major: +3 incident weight */
    _allegiance_incident_forgive  = 2, /* -1 incident weight */
};
