/* tag_name_strip_path @0x837D3C28 — return the substring after the last '\' in a tag name, or the whole name
 * if it contains no path separator. */
extern char *strrchr(const char *s, int c);
const char *tag_name_strip_path(const char *name)
{
    char *last_sep = strrchr(name, '\\');
    if ( !last_sep )
        return name;
    return last_sep + 1;
}
