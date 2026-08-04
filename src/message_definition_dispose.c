#include "headers/message_definition.h"

typedef struct _message_definition_field_reference_set _message_definition_field_reference_set;
extern void message_definition_field_reference_set_dispose(_message_definition_field_reference_set *const field_reference_set);

void message_definition_dispose(_message_definition *const message_definition)
{
    message_definition_field_reference_set_dispose(message_definition->header_fields);
    message_definition_field_reference_set_dispose(&message_definition->body_fields);
    message_definition->initialized = 0;
}
