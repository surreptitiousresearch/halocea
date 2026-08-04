/* hkString::strLen — length of a NUL-terminated string (excluding the NUL). */
int hkString_strLen(const char *src)
{
    const char *p = src;
    while (*p++)
        ;
    return (int)(p - src - 1);
}
