/* hcex_conv_bmp_name @ 0x823F24E8
   Builds the texture-manager key for a bitmap: a fixed 5-char prefix, then the
   bitmap's tag path with each '\' separator collapsed (path components folded
   into the key up to the final component), an '_' separator, the final path
   component, and finally "_<index>". The composed key is normalized by
   hcex_conv_name before return.

   Deviation: the decompiler renders the leading 5-byte prefix copy as a copy
   from an empty string literal; the exact prefix constant is not recoverable
   from the pseudocode, so it is reproduced as a 5-byte copy from `prefix` with
   this note. The manual byte-copy loops are the inlined strcpy/strcat the
   compiler emitted. */

extern int sprintf_0(char *string, const char *format, ...);
extern char *strchr(const char *s, int c);
extern char *hcex_conv_name(char *name, int max_len);

extern "C" char *hcex_conv_bmp_name(char *name, int idx, char *out, int max_len)
{
    static const char prefix[] = ""; /* 5-byte prefix (literal not recovered) */
    char index_suffix[80];

    /* copy the 5-char prefix into out */
    const char *src = prefix;
    char *dst = out - 1;
    for ( int count = 5; count; --count )
        *++dst = *++src;

    /* length of the prefix written */
    char *end = out;
    while ( *end++ )
        ;
    int length = (int)(end - out - 1);

    /* fold each backslash-separated path component into the key */
    char *component = name;
    for ( char *slash = strchr(name, '\\'); slash; slash = strchr(slash + 1, '\\') )
    {
        out[length] = *component;
        component = slash + 1;
        ++length;
    }
    out[length] = '_';
    out[length + 1] = 0;

    /* append the final path component */
    char *append = out;
    while ( *append++ )
        ;
    --append;
    int ch;
    do
    {
        ch = (unsigned char)*component++;
        *append++ = ch;
    }
    while ( ch );

    /* append "_<index>" */
    sprintf_0(index_suffix, "_%d", idx);
    char *suffix = index_suffix;
    char *tail = out;
    while ( *tail++ )
        ;
    --tail;
    do
    {
        ch = (unsigned char)*suffix++;
        *tail++ = ch;
    }
    while ( ch );

    return hcex_conv_name(out, max_len);
}
