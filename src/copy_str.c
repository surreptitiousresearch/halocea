/* copy_str @0x837662A0 — allocates a heap copy of a null-terminated string via dlMalloc. */

#include <string.h>

extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);

void * copy_str(const char *str)
{
    unsigned int length = strlen(str) + 1;
    char *result = dlMalloc(length, "D:\\Projects\\code\\HCEX\\sources\\networking\\network_sv_func.c", 0x393u);
    strcpy(result, str);
    result[length - 1] = 0;
    return result;
}
