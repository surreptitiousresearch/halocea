#ifndef DECAL_LAYER_H
#define DECAL_LAYER_H

/* decal_layer - decal render-layer index (the value carried in the rasterizer's
 * local_layer global and returned by decal_get_first_decal_index).
 * DB-verified: types_enum_values _196C929DE2B7B146782F4632B9F3AB07 carries these values verbatim. */
enum decal_layer {
	_decal_layer_primary = 0,
	_decal_layer_secondary = 1,
	_decal_layer_light = 2,
	_decal_layer_alpha_tested = 3,
	_decal_layer_water = 4,
	NUMBER_OF_DECAL_LAYERS = 5,
};

#endif /* DECAL_LAYER_H */
