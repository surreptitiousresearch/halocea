/* banlist_open_file @0x83766B20 — open the server ban list file ("banned.txt") with the given mode string and
 * return the stream. */

typedef struct _iobuf _iobuf;

extern int _snprintf_0(char *buffer, unsigned int size, const char *format, ...);
extern _iobuf *fopen(const char *filename, const char *mode);

/* DEVIATION: the buffer is the SHARED GLOBAL `result_0` (src/data/result_0.c, char[260] @
 * 0x844682F8), not a stack array. Disasm is unambiguous — `lis r10, result_0@ha` /
 * `addi r30, r10, result_0@l` / `stb r11, result_0@l(r10)`, and the frame is only 0x70 bytes,
 * far too small for 260. Modelling it as a local silently made the function reentrant. */
extern char result_0[260];

struct _iobuf * banlist_open_file(const char *how)
{
    result_0[0] = 0;
    _snprintf_0(result_0, 0x104, "%s", "banned.txt");
    return fopen(result_0, how);
}
