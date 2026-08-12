/* alphabetize_file_references @0x8372F988 — qsort comparator ordering two file references by their
 * leaf name, case-insensitively. */

#include "headers/file_reference.h"
#include "headers/name_flags.h"

extern char *file_reference_get_name(const file_reference *reference, unsigned int flags, char *name);
extern int stricmp(const char *a, const char *b);

int alphabetize_file_references(const file_reference *ref1, const file_reference *ref2)
{
    char name1[256];
    char name2[256];
    file_reference_get_name(ref1, (1u << _name_filename_bit), name1);
    file_reference_get_name(ref2, (1u << _name_filename_bit), name2);
    return stricmp(name1, name2);
}
