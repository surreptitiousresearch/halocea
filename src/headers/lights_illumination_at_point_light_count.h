#pragma once

/* lights_illumination_at_point_light_count — the number of point lights
 * lights_illumination_at_point samples when accumulating illumination at a location.
 *
 * DB-verified: types_enum_values $C7A5140B791CC8069647520C330D115C carries the single value
 * LIGHTS_ILLUMINATION_AT_POINT_LIGHT_COUNT = 2 — the anonymous enum the original source used for
 * this bound. It sizes the three parallel local arrays AND is the `maximum_count` argument passed
 * to find_point_lights_for_object_in_cluster, which is why the literal has to be one name: the
 * arrays and the cap must move together. */
enum lights_illumination_at_point_light_count
{
    LIGHTS_ILLUMINATION_AT_POINT_LIGHT_COUNT = 2,
};
