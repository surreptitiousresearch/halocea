#ifndef EFFECT_CAMERA_MODE_H
#define EFFECT_CAMERA_MODE_H

/* effect_camera_mode — the view restriction on an effect location/particle definition
 * (effect_particles_definition.camera_mode). Selects which camera a location or particle may render
 * under: independent (any), first-person only, third-person only, or both.
 * DB-verified: types_enum_values _8D839F6DDDEEC7BCD5C5A5369917AC2C carries these names verbatim. */
enum effect_camera_mode
{
    _effect_camera_mode_independent       = 0,
    _effect_camera_mode_first_person_only = 1,
    _effect_camera_mode_third_person_only = 2,
    _effect_camera_mode_both              = 3,
    NUMBER_OF_EFFECT_CAMERA_MODES         = 4,
};

#endif /* EFFECT_CAMERA_MODE_H */
