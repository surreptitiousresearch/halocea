/* message_delta_set_encoding_class — set the global message-delta encoding class (LAN vs internet).
 * The argument domain is the DB enum message_delta_processor_encoding_class. */

#include "headers/message_delta_encoding_class.h"
#include "headers/blam_data_globals.h"


void message_delta_set_encoding_class(message_delta_processor_encoding_class encoding_class)
{
    gMessageEncodingClass = encoding_class;
}
