/* DB-verified 2026-07-20 (BLOCKED-retry): values match compiled enum types_enum_values
 * _435E5D3C23E0D26C45717ED9B318C4AF exactly (DB uses the _unit_base_seat_* prefix; the _base_seat_*
 * identifiers below are the consumer-facing names and are kept). */
#ifndef BASE_SEAT_H
#define BASE_SEAT_H

/* base_seat — index into the fixed base_seat_labels table @0x841761B8 (unit_animation.base_seat_index;
 * see base_seat_label_get / seat_label_to_base_seat_index).
 * Source: the label strings read directly from .data: {"asleep","alert","stand","crouch","flee","flaming"}.
 * Ground truth. */
enum base_seat {
	_base_seat_asleep = 0,
	_base_seat_alert = 1,
	_base_seat_stand = 2,
	_base_seat_crouch = 3,
	_base_seat_flee = 4,
	_base_seat_flaming = 5,
	NUMBER_OF_BASE_SEATS = 6,
};

#endif /* BASE_SEAT_H */
