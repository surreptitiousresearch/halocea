#pragma once
/* transmission_direction — message-delta metrics account header/overhead bit costs per direction.
 * Enum member names AND values are from the database (types_enum_values).
 * FIRST_* names denote the first (0) member; NUMBER_OF_* is the trailing count sentinel. */

typedef enum transmission_direction
{
    FIRST_TRANSMISSION_DIRECTION            = 0,
    transmission_direction_sent             = 0,
    transmission_direction_received         = 1,
    NUMBER_OF_TRANSMISSION_DIRECTIONS       = 2,
} transmission_direction;
