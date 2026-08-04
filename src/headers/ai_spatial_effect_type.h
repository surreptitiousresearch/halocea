#ifndef AI_SPATIAL_EFFECT_TYPE_H
#define AI_SPATIAL_EFFECT_TYPE_H

/* ai_spatial_effect_type — kind of transient AI spatial stimulus dispatched by
 * actors_handle_spatial_effect. DB-verified: anonymous compiled enum
 * _D0FF423BE03A5FE15ED7B7AD77AA3A4C carries these values verbatim. */
enum ai_spatial_effect_type {
	_ai_spatial_effect_environmental_noise = 0,
	_ai_spatial_effect_weapon_impact = 1,
	_ai_spatial_effect_weapon_detonation = 2,
	NUMBER_OF_AI_SPATIAL_EFFECTS = 3,
};

#endif /* AI_SPATIAL_EFFECT_TYPE_H */
