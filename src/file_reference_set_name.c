/* file_reference_set_name @0x837679E8 — replace the file_reference's name component: remove the old name if a
 * name flag is set, append the new one, and re-set the flag. */
#include "headers/file_reference.h"
extern void file_path_remove_name(char *path);
extern void file_path_add_name(char *path, const char *name);
file_reference *file_reference_set_name(file_reference *reference, const char *name)
{
    if ( (*(unsigned short *)&reference->data[4] & 1) != 0 )   /* name-set flag word at data[4] */
        file_path_remove_name(&reference->data[8]);
    file_path_add_name(&reference->data[8], name);
    *(unsigned short *)&reference->data[4] |= 1u;   /* mark name set */
    return reference;
}
