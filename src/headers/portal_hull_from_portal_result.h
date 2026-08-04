#pragma once

/* portal_hull_from_portal_result — result codes of portal_hull_from_points (and the portal
 * visibility path that consumes them). DB-verified via types_enum_values
 * $052179EE31D07FABD175642A2319689A. */
enum portal_hull_from_portal_result {
	_portal_hull_from_portal_succeeded = 0,
	_portal_hull_from_portal_discarded = 1,
	_portal_hull_from_portal_degenerate = 2,
	NUMBER_OF_PORTAL_HULL_FROM_PORTAL_RESULT_CODES = 3,
};
