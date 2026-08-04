/* network_game_generate_local_machine_name @0x837829AC — fills `name` (a 16-wchar buffer) with the console's
 * Xbox Live gamertag if one is set, otherwise falls back to a randomly chosen default player name (and
 * pushes that choice back to the system as the console's nickname). Always null-terminates at index 15. */

#include <stdint.h>
#include <stddef.h>   /* wchar_t */

extern void *XFindFirstNicknameW(int flags, unsigned __int16 *name, unsigned int name_length);
extern void XFindClose(void *handle);
extern void XSetNicknameW(unsigned __int16 *name, unsigned __int8 flags);
extern uint16_t * network_game_get_random_player_name(void);
extern void ustrncpy(wchar_t *dest, const wchar_t *src, unsigned int count);

void network_game_generate_local_machine_name(uint16_t *name)
{
    void *find_handle = XFindFirstNicknameW(0, name, 0x10u);

    if ( find_handle == (void *)-1 )
    {
        const wchar_t *random_player_name = network_game_get_random_player_name();

        ustrncpy(name, random_player_name, 0x10u);
        name[15] = 0;
        XSetNicknameW(name, 1);
    }
    else
    {
        XFindClose(find_handle);
    }

    name[15] = 0;
}
