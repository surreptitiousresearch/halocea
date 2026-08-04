/* variables @ 0x841764F8 — .data data-init.
 * DB: director_variable_definition[4] (28 bytes each = 112). Brace-init from binary bytes;
 * minimum/maximum are -FLT_MAX/+FLT_MAX (0xFF7FFFFF / 0x7F7FFFFF). */
#include "headers/director_variable_definition.h"

director_variable_definition variables[4] = {
    { .negative_bit = 5, .positive_bit = 4, .reset_bit = -1,
      .scale = 0.15f, .initial_value = 0.0f,
      .minimum = -3.40282347e+38f, .maximum = 3.40282347e+38f, .has_hyper_scale = 1 },
    { .negative_bit = 6, .positive_bit = 7, .reset_bit = -1,
      .scale = 0.075f, .initial_value = 0.0f,
      .minimum = -3.40282347e+38f, .maximum = 3.40282347e+38f, .has_hyper_scale = 0 },
    { .negative_bit = 1, .positive_bit = 0, .reset_bit = -1,
      .scale = 0.075f, .initial_value = 0.0f,
      .minimum = -3.40282347e+38f, .maximum = 3.40282347e+38f, .has_hyper_scale = 1 },
    { .negative_bit = 3, .positive_bit = 2, .reset_bit = -1,
      .scale = 0.075f, .initial_value = 0.0f,
      .minimum = -3.40282347e+38f, .maximum = 3.40282347e+38f, .has_hyper_scale = 1 },
};
