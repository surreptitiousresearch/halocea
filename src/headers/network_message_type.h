#pragma once
/* network_message_type — selector for the network game server's message dispatch. */

typedef enum network_message_type
{
    network_message_type_legacy         = 0,
    network_message_type_message_delta  = 1,
    NUMBER_OF_NETWORK_MESSAGE_TYPES     = 2,
} network_message_type;
