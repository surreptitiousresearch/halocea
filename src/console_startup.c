#include <stdint.h>
#include "headers/blam_data_globals.h"
/* console_startup @0x83722D38 — run the developer console's init script (d:\init.txt, prefixed "editor_" in
 * the editor build); when running under D3D PIX/Spy with no init script, auto-load test map b30. */

extern uint8_t game_in_editor(void);
extern int _snprintf_0(char *buffer, unsigned int size, const char *format, ...);
extern char *strcpy(char *dest, const char *src);
extern uint8_t console_exec(char *filename);
extern int console_process_command(const char *command, uint16_t extra_flags);

/* DEVIATION: the decompiler threaded console_exec's r3 through to the epilogue as a return
 * value; the disasm never re-defines r3 for the return and no caller consumes it (the sole
 * caller, the thunk main_loop_init_console @0x83688C28, is itself ignored) — attested void. */
void console_startup(void)
{
    char init_path[136];
    if ( game_in_editor() )
        /* backslash escaped 2026-07-30: "\i" silently dropped the '\' (DB string 0x82114E44 = d:\init.txt) */
        _snprintf_0(init_path, 0x7Fu, "editor_%s", "d:\\init.txt");
    else
        strcpy(init_path, "d:\\init.txt");

    if ( !console_exec(init_path) && UsingD3DSpy )
        console_process_command("map_name b30", 0);
}
