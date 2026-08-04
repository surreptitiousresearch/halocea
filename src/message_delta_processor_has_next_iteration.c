#include "headers/message_delta_processor_header.h"

int message_delta_processor_has_next_iteration(const message_delta_processor_header *const header)
{
    return header->decoding_information->current_iteration <=
           header->decoding_information->iteration_count;
}
