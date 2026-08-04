#ifndef CONTRAIL_POINT_DATUM_FLAGS_H
#define CONTRAIL_POINT_DATUM_FLAGS_H

/* contrail_point_datum_flags — bit indices for contrail_point_datum.flags (runtime contrail point state;
 * point->flags in contrail_update_points.c / render_contrail.c). Verified against contrail_update_points.c:
 * bit0 just-spawned (new), bit1 accelerating sub-stage (transitioning), bit2 dead (living_dead).
 * Source: reference enum _C177D0346B9EEAB2C4C4A4DE08FD8E7A (headers_ref). Values are BIT INDICES — use as (1u << _bit). */
enum contrail_point_datum_flags {
	_contrail_point_new_bit = 0x0,
	_contrail_point_transitioning_bit = 0x1,
	_contrail_point_living_dead_bit = 0x2,
	NUMBER_OF_CONTRAIL_POINT_FLAGS = 0x3,
};

#endif /* CONTRAIL_POINT_DATUM_FLAGS_H */

// contrail_point_datum_flags: DB-verified via types_enum_values _C177D0346B9EEAB2C4C4A4DE08FD8E7A (anonymous compiled enum, names sans leading _)
