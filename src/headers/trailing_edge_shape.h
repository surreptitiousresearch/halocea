#ifndef TRAILING_EDGE_SHAPE_H
#define TRAILING_EDGE_SHAPE_H

/* trailing_edge_shape — flag_definition.trailing_edge_shape (the cloth's free-edge silhouette).
 * layout bound to DB enum: types_enum_values _BA8F4C162F2F0D6AF1ED4D7695D2DB21 */
enum trailing_edge_shape {
	_trailing_edge_shape_flat = 0,
	_trailing_edge_shape_concave_triangular = 1,
	_trailing_edge_shape_convex_triangular = 2,
	_trailing_edge_shape_trapezoid_short_top = 3,
	_trailing_edge_shape_trapezoid_short_bottom = 4,
	NUMBER_OF_TRAILING_EDGE_SHAPES = 5,
};

#endif /* TRAILING_EDGE_SHAPE_H */
