#ifndef ATTACHED_EDGE_SHAPE_H
#define ATTACHED_EDGE_SHAPE_H

/* attached_edge_shape — flag_definition.attached_edge_shape (the cloth's attached-edge silhouette).
 * layout bound to DB enum: types_enum_values _2C19CE56D09949CBA6CF04B429FAE4BB */
enum attached_edge_shape {
	_attached_edge_shape_flat = 0,
	_attached_edge_shape_triangular_notch = 1,
	NUMBER_OF_ATTACHED_EDGE_SHAPES = 2,
};

#endif /* ATTACHED_EDGE_SHAPE_H */
