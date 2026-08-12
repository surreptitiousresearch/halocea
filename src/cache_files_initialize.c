#include <stdint.h>
#include "headers/cache_file_globals.h"
#include "headers/cache_request.h"
#include "headers/blam_data_globals.h"

extern uint8_t data_file_open(uint32_t data_file_id, const char *file_name, uint8_t writable);
extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);
extern void cache_file_windows_thread_proc(void);
void       *CreateEventA(void *attributes, int manual_reset, int initial_state, const char *name);
void       *CreateThread(void *attributes, uint32_t stack_size,
                         uint32_t (*start)(void *), void *param,
                         uint32_t flags, uint32_t *thread_id);


/* DEVIATION: the DB prototype says `void *` (the CreateThread HANDLE is still in r3 at the exit),
 * but the binary computes no return value: the handle is STORED (stw r3@0x83755164) and merely left
 * behind in r3 at blr@0x83755178, and no consumer exists anywhere — the only path in is the 1-insn
 * tail-b thunk tag_files_open@0x836F6F40, whose single caller shell_initialize@0x836FBDB4 ignores r3.
 * Both spellings emit identical code, so the image cannot falsify `void *`; the locked
 * "0 consumers + r3 at blr only from callee => void" rule decides, and a DB prototype is a hint. */
void cache_files_initialize(void)
{
    void *thread;

    cache_file_globals_0.open_map_file_index = -1;
    data_file_open(1u, "bitmaps", 0);
    cache_file_globals_0.requests = dlMalloc(
        0x6000u,
        "D:\\Projects\\code\\HCEX\\sources\\cache\\cache_files_windows.c",
        0xF5u);
    cache_file_globals_0.sleep_event = CreateEventA(0, 0, 0, 0);
    thread = CreateThread(
        0,
        0x4000u,
        /* faithful Win32 boundary pun: the real proc is void(void) (never returns, ignores its
         * param); cast to the LPTHREAD_START_ROUTINE shape the API requires, as in the binary. */
        (uint32_t (*)(void *))cache_file_windows_thread_proc,
        0,
        0,
        0);
    cache_file_globals_0.thread = thread;
}
