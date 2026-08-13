#pragma once

/* geometry_constants — geometry cardinality constants: component indices, vertices/edges/faces per primitive, rectangle corner indices (the DB carries both QUADRALATERAL and QUADRILATERAL spellings — kept verbatim).
 * Source: types_enum_values $4EFC0AB65FE8DB653280558E66CA0FFE (DB-verified, members verbatim in DB order). */
enum geometry_constants {
	_x = 0,
	_y = 1,
	_z = 2,
	NUMBER_OF_RECTANGLE2D_COMPONENTS = 4,
	NUMBER_OF_RECTANGLE3D_COMPONENTS = 6,
	NUMBER_OF_VERTICES_PER_LINE = 2,
	NUMBER_OF_VERTICES_PER_TRIANGLE = 3,
	NUMBER_OF_VERTICES_PER_QUADRALATERAL = 4,
	NUMBER_OF_VERTICES_PER_QUADRILATERAL = 4,
	NUMBER_OF_VERTICES_PER_HEXAGON = 6,
	NUMBER_OF_VERTICES_PER_PYRAMID = 5,
	NUMBER_OF_VERTICES_PER_CUBE = 8,
	NUMBER_OF_TRIANGLES_PER_QUADRILATERAL = 2,
	NUMBER_OF_EDGES_PER_TRIANGLE = 3,
	NUMBER_OF_EDGES_PER_QUADRALATERAL = 4,
	NUMBER_OF_EDGES_PER_HEXAGON = 6,
	NUMBER_OF_FACES_PER_CUBE = 6,
	_rectangle_top_left = 0,
	_rectangle_bottom_left = 1,
	_rectangle_top_right = 2,
	_rectangle_bottom_right = 3,
	NUMBER_OF_POINTS_PER_RECTANGLE = 4,
};

// geometry_constants: DB-verified via types_enum_values $4EFC0AB65FE8DB653280558E66CA0FFE
