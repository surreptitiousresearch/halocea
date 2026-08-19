/* message_delta_metrics_dump @0x837A07B8 — write the accumulated network-message metrics to a file,
 * defaulting the name to "Message Summary.txt" when none (or an empty string) is given.
 *
 * DEVIATION: the decompiler inlines the empty-string test as a pointer-walk strlen; reproduced as the
 * equivalent check. */

typedef struct real_argb_color real_argb_color;
extern void metrics_generate_filename(const char *base, char *destination, unsigned int max_size);
extern void metrics_dump(const char *filename);
extern void terminal_printf(const real_argb_color *color, const char *format, ...);

void message_delta_metrics_dump(const char *filename)
{
    if ( !filename || !*filename )
        filename = "Message Summary.txt";

    char path[264];
    metrics_generate_filename(filename, path, 260);
    metrics_dump(path);
    terminal_printf(0, "Wrote network message metrics to %s", path);
}
