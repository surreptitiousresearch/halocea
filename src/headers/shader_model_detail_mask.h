#ifndef SHADER_MODEL_DETAIL_MASK_H
#define SHADER_MODEL_DETAIL_MASK_H

/* shader_model_detail_mask — values of shader_model.model.detail_mask (which mask channel the
 * detail map modulates). DB-verified via types_enum_values $C22432BFF1841B370618AEC6D1CEDB15. */
enum shader_model_detail_mask {
	_shader_model_detail_mask_none = 0,
	_shader_model_detail_mask_reflection_mask_inverse = 1,
	_shader_model_detail_mask_reflection_mask = 2,
	_shader_model_detail_mask_self_illumination_mask_inverse = 3,
	_shader_model_detail_mask_self_illumination_mask = 4,
	_shader_model_detail_mask_change_color_mask_inverse = 5,
	_shader_model_detail_mask_change_color_mask = 6,
	_shader_model_detail_mask_multipurpose_alpha_mask_inverse = 7,
	_shader_model_detail_mask_multipurpose_alpha_mask = 8,
	NUMBER_OF_SHADER_MODEL_DETAIL_MASKS = 9,
};

#endif /* SHADER_MODEL_DETAIL_MASK_H */
