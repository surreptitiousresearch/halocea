#ifndef MODEL_DETAIL_LEVEL_H
#define MODEL_DETAIL_LEVEL_H

/* model_detail_level — model geometry LOD (detail level) indices, super-low..super-high. */
enum model_detail_level {
	_detail_level_super_low = 0,
	_detail_level_low = 1,
	_detail_level_medium = 2,
	_detail_level_high = 3,
	_detail_level_super_high = 4,
	NUMBER_OF_DETAIL_LEVELS_PER_MODEL = 5,
};

#endif /* MODEL_DETAIL_LEVEL_H */

// model_detail_level: DB-verified via types_enum_values $DACCBF4E742CCBF05044CABA4039C89B
