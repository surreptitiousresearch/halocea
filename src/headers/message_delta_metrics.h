#pragma once
/* message_delta_metrics — instrumentation enums and entry points used by the message-delta encoder to
 * account header/overhead bit costs. Enum member names AND values are from the database (types_enum_values).
 * FIRST_* names denote the first (0) member of their enum; NUMBER_OF_* is the trailing count sentinel. */

#include "message_definition.h"
#include "iterated_message.h"
/* These enums have their own authoritative sibling headers — include, do not redefine. */
#include "transmission_direction.h"
#include "iteration_dependent_overhead_type.h"
#include "iteration_independent_overhead_type.h"

typedef struct _message_definition _message_definition;
extern void metrics_commit_header_field_data(const _message_definition *message_definition, message_delta_processor_mode mode, transmission_direction direction);
extern void metrics_rollback_header_field_data(const _message_definition *message_definition, message_delta_processor_mode mode, transmission_direction direction);
extern void metrics_record_iteration_dependent_overhead_encode(const _message_definition *definition, message_delta_processor_mode mode, iteration_dependent_overhead_type type, int bits);
extern void metrics_record_iteration_independent_overhead_encode(const _message_definition *definition, message_delta_processor_mode mode, iteration_independent_overhead_type type, int bits);
extern void metrics_record_message_encode(const _message_definition *definition, message_delta_processor_mode mode, int total_bits, int iterations_written);
