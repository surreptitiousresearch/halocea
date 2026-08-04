#ifndef AI_LINE_OF_SIGHT_H
#define AI_LINE_OF_SIGHT_H

/* prop_datum.line_of_sight / firing_position.line_of_sight domain.
 * DB anonymous enum _539275654DB971F8506FCC6E69D7C443. */
enum ai_line_of_sight {
	_ai_line_of_sight_clear = 0,
	_ai_line_of_sight_occluded = 1,
	_ai_line_of_sight_from_cover = 2,
	_ai_line_of_sight_to_cover = 3,
	_ai_line_of_sight_obstructed = 4,
	NUMBER_OF_AI_LINE_OF_SIGHTS = 5,
};

#endif /* AI_LINE_OF_SIGHT_H */
