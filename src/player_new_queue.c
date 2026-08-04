#include "headers/update_server_globals.h"

#include "headers/data_array.h"
extern int datum_new_at_index(data_array *data, int index);

int player_new_queue(int player_index)
{
    return datum_new_at_index(update_server_globals.queues, player_index);
}
