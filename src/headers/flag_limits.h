#ifndef FLAG_LIMITS_H
#define FLAG_LIMITS_H

/* Flag (cloth) simulation size limits — a definition exceeding these is rejected (rendered inert).
 * layout bound to DB enum: types_enum_values _A72AC1894B41DC751C4847A36602A92A */
enum {
	MAXIMUM_FLAG_VERTEX_AREA = 225,   /* height * width */
	MAXIMUM_FLAG_CELL_AREA = 196,
	MAXIMUM_FLAG_HEIGHT = 40,
	FLAG_MATERIAL_COUNT = 1,
};

#endif /* FLAG_LIMITS_H */
