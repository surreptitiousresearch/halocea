/* file_reference_add_directory @0x837679B0 — append a directory component to a file reference's path (the path
 * string begins at byte 8 of the reference) and return the reference. */

#include "headers/file_reference.h"

extern void file_path_add_name(char *path, const char *name);

file_reference *file_reference_add_directory(file_reference *reference, const char *directory)
{
    file_path_add_name(reference->info.path, directory);
    return reference;
}
