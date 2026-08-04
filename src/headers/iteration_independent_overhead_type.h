#pragma once

typedef enum iteration_independent_overhead_type
{
    FIRST_ITERATION_INDEPENDENT_OVERHEAD_TYPE = 0,
    iteration_independent_overhead_message_mode = 0,
    iteration_independent_overhead_message_id = 1,
    iteration_independent_overhead_protocol = 2,
    iteration_independent_overhead_iterations = 3,
    iteration_independent_overhead_padding = 4,
    NUMBER_OF_ITERATION_INDEPENDENT_OVERHEAD_TYPES = 5,
} iteration_independent_overhead_type;
