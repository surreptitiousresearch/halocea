#pragma once
/* director camera-mode slot — the camera-mode index stored in director.camera_mode_index.
 * DB-AUTHORITATIVE: types_enum_values _3F888D5C22E2E134A82E24460B64DC75. */
enum director_camera_mode
{
    _camera_following    = 0,
    _camera_orbiting     = 1,
    _camera_flying       = 2,
    _camera_editor       = 3,
    _camera_first_person = 4,
    NUMBER_OF_DIRECTOR_CAMERA_MODES = 5,
};
