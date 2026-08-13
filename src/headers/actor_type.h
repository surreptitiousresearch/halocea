/* DB-verified 2026-07-20 (BLOCKED-retry): values match compiled enum types_enum_values
 * _568501142F41B43EEF44B569953A20C1 exactly (DB uses the _actor_* prefix, e.g. _actor_elite; the
 * _actor_type_* identifiers below are the consumer-facing names and are kept). */
#ifndef ACTOR_TYPE_H
#define ACTOR_TYPE_H

/* actor_type — AI character class stored in _actor_datum.meta.type (see actor_new @actor->meta.type = ,
 * and the actor_type_get_* accessors keyed by meta.type). Also the value of prop.type when a prop mirrors
 * an actor's class (actor_emotion_flee_with_friends compares prop->type == actor->meta.type).
 *
 * NOT in headers_ref/DB as an enum (the member is a plain __int16). Names recovered from the binary's
 * own data: this is one of the four HaloScript-facing enums registered in the hs enum-type block
 * @0x8211C9C4; its ordered name-string table is @0x84185320:
 *   elite[0] jackal[1] grunt[2] hunter[3] engineer[4] assassin[5] player[6] marine[7] crew[8]
 *   combat_form[9] infection_form[10] carrier_form[11] monitor[12] sentinel[13] none[14]
 *   mounted_weapon[15]
 * Corroborated by actor_type_get_name @0x837F8E20 (indexes this table). */
enum actor_type {
	_actor_type_elite = 0,
	_actor_type_jackal = 1,
	_actor_type_grunt = 2,
	_actor_type_hunter = 3,
	_actor_type_engineer = 4,
	_actor_type_assassin = 5,
	_actor_type_player = 6,
	_actor_type_marine = 7,
	_actor_type_crew = 8,
	_actor_type_combat_form = 9,
	_actor_type_infection_form = 10,
	_actor_type_carrier_form = 11,
	_actor_type_monitor = 12,
	_actor_type_sentinel = 13,
	_actor_type_none = 14,
	_actor_type_mounted_weapon = 15,
	NUMBER_OF_ACTOR_TYPES = 16,
};

#endif /* ACTOR_TYPE_H */
// actor_type: independently attested by hs_enum_table @0x8211C9C4 record 3 — the name
// table @0x84185320 spells all 16 types (elite..mounted_weapon) in this exact order
// (decoded 2026-08-13, enum drain round 2).
