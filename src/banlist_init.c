/* banlist_init @ 0x83767090 — load banned.txt into the banned-players array.
 * Each line is "name,id"; entries already banned are skipped. */

#include <stdint.h>
#include <string.h>
#include "headers/dynamic_array.h"
#include "headers/banned_player_s.h"

#include <stdio.h>
#include "headers/blam_data_globals.h"

extern int _snprintf_0(char *, unsigned int, const char *, ...);
extern void trim_whitespace(char **str);
extern uint8_t _banlist_player_is_banned(const char *hash);
/* strncpy declared by <string.h> */

void banlist_init(void)
{
    FILE *file;
    char line[576];
    char *cursor;

    result_0[0] = 0;
    _snprintf_0(result_0, 0x104, "%s", "banned.txt");
    file = fopen(result_0, "rt");
    dynamic_array_new(&banned_players, 46);  /* 13-byte name + 33-byte id */
    if ( !file )
        return;

    while ( fgets(line, 512, file) )
    {
        char *name;
        char *comma;
        const char *id;
        int idx;

        cursor = line;
        trim_whitespace(&cursor);
        name = cursor;
        comma = strchr(cursor, ',');
        if ( !comma )
            break;
        *comma = 0;
        id = comma + 1;
        /* attested callee: binary calls _banlist_player_is_banned(const char *hash);
         * reconstruction had called the unrelated int-keyed thunk with a spurious ptr->int cast */
        if ( _banlist_player_is_banned(id) )
            continue;

        idx = dynamic_array_add_element(&banned_players);
        if ( idx != -1 )
        {
            banned_player_s *entry = &((banned_player_s *)banned_players.elements)[idx];
            strncpy(entry->name, name, 0xC);
            entry->name[12] = 0;
            strncpy(entry->hash, id, 0x20);
            entry->hash[32] = 0;
        }
    }
    fclose(file);
}
