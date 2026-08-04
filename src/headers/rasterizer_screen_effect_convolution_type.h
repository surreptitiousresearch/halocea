#ifndef RASTERIZER_SCREEN_EFFECT_CONVOLUTION_TYPE_H
#define RASTERIZER_SCREEN_EFFECT_CONVOLUTION_TYPE_H

/* rasterizer_screen_effect_convolution_type — convolution kind stored in
 * rasterizer_screen_effect_parameters.convolution_type.
 * DB-verified: types_enum_values _BF0C968D74B27769F9A988511BB8ED9D carries these values verbatim. */
enum rasterizer_screen_effect_convolution_type {
	_rasterizer_screen_effect_convolution_type_none = 0,
	_rasterizer_screen_effect_convolution_type_blur = 1,
	_rasterizer_screen_effect_convolution_type_warp = 2,
	NUMBER_OF_RASTERIZER_SCREEN_EFFECT_CONVOLUTION_TYPES = 3,
};

#endif /* RASTERIZER_SCREEN_EFFECT_CONVOLUTION_TYPE_H */
