#ifndef UNIT_GRENADE_TYPE_H
#define UNIT_GRENADE_TYPE_H

/* unit_grenade_type — the grenade types a unit can carry (indices into
 * _unit_datum.grenade_counts[NUMBER_OF_UNIT_GRENADE_TYPES]).
 * DB-verified: types_enum_values _C04139DE66CD4C6C6A695EB26A7024EE (anonymous
 * compiled enum) carries these values verbatim. */
enum unit_grenade_type {
	_unit_grenade_human_fragmentation = 0,
	_unit_grenade_covenant_plasma = 1,
	NUMBER_OF_UNIT_GRENADE_TYPES = 2,
};

#endif /* UNIT_GRENADE_TYPE_H */
