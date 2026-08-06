/* variables @ 0x841764F8 (.data, 112 bytes)
 * DB applied_types: director_variable_definition variables[4];
 * Image bytes (big-endian), decoded from the binary .data record:
 *   [ 0] +0x00 negative_bit               = 0x0005
 *        +0x02 positive_bit               = 0x0004
 *        +0x04 reset_bit                  = 0xFFFF
 *        +0x08 scale                      = 0x3E19999A -> 0.15f
 *        +0x0C initial_value              = 0x00000000 -> 0f
 *        +0x10 minimum                    = 0xFF7FFFFF -> -3.40282e+38f
 *        +0x14 maximum                    = 0x7F7FFFFF -> 3.40282e+38f
 *        +0x18 has_hyper_scale            = 0x01
 *   [ 1] +0x00 negative_bit               = 0x0006
 *        +0x02 positive_bit               = 0x0007
 *        +0x04 reset_bit                  = 0xFFFF
 *        +0x08 scale                      = 0x3D99999A -> 0.075f
 *        +0x0C initial_value              = 0x00000000 -> 0f
 *        +0x10 minimum                    = 0xFF7FFFFF -> -3.40282e+38f
 *        +0x14 maximum                    = 0x7F7FFFFF -> 3.40282e+38f
 *        +0x18 has_hyper_scale            = 0x00
 *   [ 2] +0x00 negative_bit               = 0x0001
 *        +0x02 positive_bit               = 0x0000
 *        +0x04 reset_bit                  = 0xFFFF
 *        +0x08 scale                      = 0x3D99999A -> 0.075f
 *        +0x0C initial_value              = 0x00000000 -> 0f
 *        +0x10 minimum                    = 0xFF7FFFFF -> -3.40282e+38f
 *        +0x14 maximum                    = 0x7F7FFFFF -> 3.40282e+38f
 *        +0x18 has_hyper_scale            = 0x01
 *   [ 3] +0x00 negative_bit               = 0x0003
 *        +0x02 positive_bit               = 0x0002
 *        +0x04 reset_bit                  = 0xFFFF
 *        +0x08 scale                      = 0x3D99999A -> 0.075f
 *        +0x0C initial_value              = 0x00000000 -> 0f
 *        +0x10 minimum                    = 0xFF7FFFFF -> -3.40282e+38f
 *        +0x14 maximum                    = 0x7F7FFFFF -> 3.40282e+38f
 *        +0x18 has_hyper_scale            = 0x01
 * .data data-init.
 * DB: director_variable_definition[4] (28 bytes each = 112). Brace-init from binary bytes;
 * minimum/maximum are -FLT_MAX/+FLT_MAX (0xFF7FFFFF / 0x7F7FFFFF).
 */
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
