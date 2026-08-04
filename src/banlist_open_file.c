/* banlist_open_file @0x83766B20 — open the server ban list file ("banned.txt") with the given mode string and
 * return the stream. */

typedef struct _iobuf _iobuf;

extern int _snprintf_0(char *buffer, unsigned int size, const char *format, ...);
extern _iobuf *fopen(const char *filename, const char *mode);

struct _iobuf * banlist_open_file(const char *how)
{
    char path[260];
    path[0] = 0;
    _snprintf_0(path, 0x104, "%s", "banned.txt");
    return fopen(path, how);
}
