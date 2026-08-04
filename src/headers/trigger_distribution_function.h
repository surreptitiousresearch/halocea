#ifndef TRIGGER_DISTRIBUTION_FUNCTION_H
#define TRIGGER_DISTRIBUTION_FUNCTION_H

/* weapon trigger projectile distribution function (spread pattern).
 * DB-verified: types_enum_values _CBD5B9C11615EA18D32886DF56F9B3E4 carries these names verbatim. */
enum trigger_distribution_function {
	_trigger_distribution_point = 0,
	_trigger_distribution_horizontal_fan = 1,
	NUMBER_OF_TRIGGER_DISTRIBUTION_FUNCTIONS = 2,
};

#endif /* TRIGGER_DISTRIBUTION_FUNCTION_H */
