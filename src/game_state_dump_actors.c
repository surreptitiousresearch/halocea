/* game_state_dump_actors @0x83684990 — debug dump of the entire actor data array to
 * d:\actorsNN.dat. No-op if the file can't be opened. */

#include <stdio.h>
#include "headers/blam_data_globals.h"

void game_state_dump_actors(int idx)
{
    char filename[64];
    sprintf(filename, "d:\\actors%02d.dat", idx);
    FILE *file = fopen(filename, "wb");
    if ( file )
    {
        fwrite(actor_data->data, actor_data->count * actor_data->size, 1u, file);
        fclose(file);
    }
}
