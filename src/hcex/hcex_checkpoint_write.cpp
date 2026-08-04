/* hcex_checkpoint_write @0x83684DF0 — write a checkpoint/autosave blob to the autosave file for the current
 * scenario. Resolves the autosave path from the scenario tag name, (re)creates the file, truncates it to
 * 4.5 MB, writes the buffer from the start, and on a fully-written buffer records it as the current
 * checkpoint. Returns TRUE on success. */

#include <stdint.h>

extern int global_scenario_index;
extern char *tag_get_name(int16_t tag_index);
extern const char *hcex_get_autosave_name(const char *scenario_name);
extern void hcex_set_current_checkpoint(const char *autosave_name);
extern void *CreateFileA(const char *name, unsigned int access, unsigned int share, void *security,
    unsigned int creation, unsigned int flags, void *template_file);
extern unsigned int SetFilePointer(void *file, int distance, int *distance_high, unsigned int method);
extern int SetEndOfFile(void *file);
extern int WriteFile(void *file, const void *buffer, unsigned int count, unsigned int *written, void *overlapped);
extern int CloseHandle(void *file);

#define GENERIC_READ              0x80000000u
#define GENERIC_WRITE             0x40000000u
#define OPEN_ALWAYS               4u
#define FILE_FLAG_SEQUENTIAL_SCAN 0x08000000u
#define INVALID_HANDLE_VALUE      ((void *)-1)
#define AUTOSAVE_FILE_SIZE        4718592 /* 0x480000 */

extern "C" int hcex_checkpoint_write(void *buffer, unsigned int buffer_size)
{
    int succeeded = 0;
    const char *scenario_name = tag_get_name(global_scenario_index);
    const char *autosave_name = hcex_get_autosave_name(scenario_name);
    if ( autosave_name )
    {
        void *file = CreateFileA(autosave_name, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_ALWAYS,
                                 FILE_FLAG_SEQUENTIAL_SCAN, 0);
        if ( file == INVALID_HANDLE_VALUE )
            return 0;
        if ( SetFilePointer(file, AUTOSAVE_FILE_SIZE, 0, 0) != -1 && SetEndOfFile(file) && SetFilePointer(file, 0, 0, 0) != -1 )
        {
            unsigned int written;
            WriteFile(file, buffer, buffer_size, &written, 0);
            succeeded = buffer_size == written;
        }
        CloseHandle(file);
    }
    if ( succeeded )
        hcex_set_current_checkpoint(autosave_name);
    return succeeded;
}
