#pragma once
/* director perspective mode — the camera perspective a local player's director reports
 * (director.perspective / director_get_perspective / director_desired_perspective).
 * DB-AUTHORITATIVE: types_enum_values _6FED6EE31870DB6100D9B2D6B0E5E533. */
enum director_perspective
{
    _director_perspective_first_person = 0,
    _director_perspective_third_person = 1,
    _director_perspective_scripted     = 2,
    _director_perspective_neutral      = 3,
    NUMBER_OF_DIRECTOR_PERSPECTIVE_MODES = 4,
};
