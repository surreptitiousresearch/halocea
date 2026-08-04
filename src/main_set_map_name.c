/* main_set_map_name @0x83688E48 — set the solo map to load by name: clear the want-main-menu flag, copy the
 * name (bounded), enable persistent-storage loading, and — when not in a network connection and the editor or
 * a game is running — request a map rename. */

#include <stdint.h>
#include "headers/main_globals.h"

extern char *strncpy(char *dst, const char *src, unsigned int count);
extern uint8_t game_in_editor(void);
extern uint8_t game_in_progress(void);

void main_set_map_name(const char *name)
{
    main_globals.want_to_be_at_main_menu = 0;
    strncpy(main_globals.soloplayer_map_name, name, 0xFFu);
    main_globals.soloplayer_map_name[255] = 0;
    main_globals.solo_try_and_load_from_persistent_storage = 1;
    if ( !main_globals.connection && (game_in_editor() || game_in_progress()) )
        main_globals.rename_map = 1;
}
