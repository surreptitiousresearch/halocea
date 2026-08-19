/* alphabetize @0x8372F148 — qsort comparator: case-insensitive compare of two string pointers. */

extern int stricmp(const char *a, const char *b);

int alphabetize(const char **string1, const char **string2)
{
    return stricmp(*string1, *string2);
}
