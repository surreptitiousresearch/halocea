#ifndef RENDER_WIDGET_TYPE_H
#define RENDER_WIDGET_TYPE_H

/* render_widget_type — object render-widget classes (rasterizer_widget_begin type argument).
 * DB-verified: types_enum_values _480250086721BFB5732D1F7640E22095 carries these named values
 * verbatim. The first five (0..4) are the tag-visible widget types; 5..7 are engine-internal. */
enum render_widget_type {
	_widget_type_flag = 0,
	_widget_type_antenna = 1,
	_widget_type_glow = 2,
	_widget_type_light_volume = 3,
	_widget_type_lightning = 4,
	NUMBER_OF_WIDGET_TYPES = 5,
	_widget_type_internal_sprite = 5,
	_widget_type_internal_occlusion_test = 6,
	_widget_type_internal____ = 7,
	NUMBER_OF_INTERNAL_WIDGET_TYPES = 8,
};

#endif /* RENDER_WIDGET_TYPE_H */
