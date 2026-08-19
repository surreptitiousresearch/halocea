/* game_state_dump_actors @0x83684990 — debug dump of the entire actor data array to
 * d:\actorsNN.dat. No-op if the file can't be opened. */

#include <stdio.h>
#include "headers/blam_data_globals.h"

extern int sprintf_0(char *string, const char *format, ...);

void game_state_dump_actors(int idx)
{
    char filename[64];
    /* DEVIATION: callee is sprintf_0 per bl @0x836849B0 (no plain sprintf symbol exists) */
    sprintf_0(filename, "d:\\actors%02d.dat", idx);
    FILE *file = fopen(filename, "wb");
    if ( file )
    {
        fwrite(actor_data->data, actor_data->count * actor_data->size, 1u, file);
        fclose(file);
    }
}
