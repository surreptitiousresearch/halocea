/* metrics_record_message_encode @0x83818D80 — empty stub (bare blr). Message-delta metrics instrumentation
 * is compiled out in this build; kept as a real definition so message_finished_encoding links. */

#include "headers/message_delta_metrics.h"

void metrics_record_message_encode(const _message_definition *definition, message_delta_processor_mode mode,
                                   int total_bits, int iterations_written)
{
}
