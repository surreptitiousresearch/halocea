/* hcex_checkpoint_read @0x83684F94 — read the current HCEX checkpoint file into `buffer`. The whole file
 * must read (bytes read == buffer_size) for success. On a successful read, if the checkpoint is not itself
 * the "_autosave.sav" file, the checkpoint is mirrored into "<checkpoint-dir>\_autosave.sav": that file is
 * created (OPEN_ALWAYS), pre-sized to 0x480000 bytes via SetEndOfFile, rewound, and the buffer written to
 * it. Returns 1 on a successful read, 0 if there is no checkpoint, the file cannot be opened, or it does
 * not read fully.
 *
 * DEVIATION: the compiler inlined strlen (the `while (*p++)` length walk), strcmp (the byte-compare
 * do-while against "_autosave.sav"), and strcat (the manual append loop); these are written here as the
 * library calls they represent. strrchr and strncpy are real calls in the binary. */

extern "C" const char *hcex_get_current_checkpoint(void);
extern "C" void *CreateFileA(const char *file_name, unsigned int desired_access, unsigned int share_mode,
                         void *security_attributes, unsigned int creation_disposition,
                         unsigned int flags_and_attributes, void *template_file);
extern "C" unsigned int SetFilePointer(void *file, int distance, int *distance_high, unsigned int method);
extern "C" int ReadFile(void *file, void *buffer, unsigned int count, unsigned int *read, void *overlapped);
extern "C" int WriteFile(void *file, const void *buffer, unsigned int count, unsigned int *written, void *overlapped);
extern "C" int SetEndOfFile(void *file);
extern "C" int CloseHandle(void *handle);
extern "C" char *strrchr(const char *s, int c);
extern "C" char *strncpy(char *dst, const char *src, unsigned int n);
extern "C" char *strcat(char *dst, const char *src);
extern "C" int strcmp(const char *a, const char *b);
extern "C" unsigned int strlen(const char *s);

#define GENERIC_READ              0x80000000u
#define GENERIC_WRITE             0x40000000u
#define OPEN_EXISTING             3u
#define OPEN_ALWAYS               4u
#define FILE_FLAG_SEQUENTIAL_SCAN 0x08000000u
#define INVALID_HANDLE_VALUE      ((void *)-1)
#define AUTOSAVE_FILE_SIZE        4718592 /* 0x480000 */

extern "C" int hcex_checkpoint_read(void *buffer, unsigned int buffer_size)
{
    const char *checkpoint = hcex_get_current_checkpoint();
    if (!checkpoint)
        return 0;

    void *file = CreateFileA(checkpoint, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, 0);
    if (file == INVALID_HANDLE_VALUE)
        return 0;

    int ok = 0;
    unsigned int bytes_read;
    if (SetFilePointer(file, 0, 0, 0) != (unsigned int)-1
        && ReadFile(file, buffer, buffer_size, &bytes_read, 0))
        ok = (bytes_read == buffer_size);
    CloseHandle(file);
    if (!ok)
        return ok;

    /* mirror the checkpoint into "<dir>\_autosave.sav" unless it already is the autosave file */
    const char *filename = strrchr(checkpoint, '\\');
    if (filename && strlen(checkpoint) <= 1000 && strcmp(filename + 1, "_autosave.sav") != 0)
    {
        char autosave_path[1088];
        int dir_length = filename - checkpoint;
        strncpy(autosave_path, checkpoint, dir_length + 1);
        autosave_path[dir_length + 1] = 0;
        strcat(autosave_path, "_autosave.sav");

        void *autosave = CreateFileA(autosave_path, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_ALWAYS,
                                     FILE_FLAG_SEQUENTIAL_SCAN, 0);
        if (autosave != INVALID_HANDLE_VALUE)
        {
            unsigned int bytes_written;
            if (SetFilePointer(autosave, AUTOSAVE_FILE_SIZE, 0, 0) != (unsigned int)-1
                && SetEndOfFile(autosave)
                && SetFilePointer(autosave, 0, 0, 0) != (unsigned int)-1)
                WriteFile(autosave, buffer, buffer_size, &bytes_written, 0);
            CloseHandle(autosave);
            return ok;
        }
    }

    return 1;
}
