/* file_reference_set_name @0x837679E8 — replace the file_reference's name component: remove the old name if a
 * name flag is set, append the new one, and re-set the flag. */
#include "headers/file_reference.h"
#include "headers/reference_info_flags.h"
extern void file_path_remove_name(char *path);
extern void file_path_add_name(char *path, const char *name);
file_reference *file_reference_set_name(file_reference *reference, const char *name)
{
    if ( (reference->info.flags & (1u << _has_filename_bit)) != 0 )
        file_path_remove_name(reference->info.path);
    file_path_add_name(reference->info.path, name);
    reference->info.flags |= (1u << _has_filename_bit);   /* mark name set */
    return reference;
}
