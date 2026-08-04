#include "headers/update_client_globals.h"

#include "headers/data_array.h"
extern int datum_new_at_index(data_array *data, int index);

int update_client_add_player(int player_index)
{
    return datum_new_at_index(update_client_globals.queues, player_index);
}
