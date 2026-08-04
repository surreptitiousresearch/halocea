#ifndef TRIGGER_FIRING_EFFECT_TYPE_H
#define TRIGGER_FIRING_EFFECT_TYPE_H

/* trigger_firing_effect_type — index into trigger_firing_effect.effects[3]/damage_effects[3]
 * (which of the barrel's effect slots a shot resolves to: normal fire, overheated/misfire, empty). */
enum trigger_firing_effect_type {
	_trigger_firing_effect = 0,
	_trigger_overheated_effect = 1,
	_trigger_empty_effect = 2,
	NUMBER_OF_TRIGGER_FIRING_EFFECTS = 3,
};

#endif /* TRIGGER_FIRING_EFFECT_TYPE_H */

// trigger_firing_effect_type: DB-verified via types_enum_values $A24F0A970643CEAB26651161149A6E62
