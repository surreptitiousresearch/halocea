/* hs_rebuild_source_file @0x8372F8A8 — read a HaloScript source file off disk and append it as a new element
 * of the scenario's hs_source_files block (so the compiled scenario records its source). Each element is 52
 * bytes: a 31-char name (NUL-terminated at +31) followed by a tag_data holding the file contents at +32.
 * Returns 1 on success, 0 on any failure. */

#include <stdint.h>
#include <string.h>
#include "headers/scenario.h"
#include "headers/hs_source_file.h"
#include "headers/file_reference.h"
#include "headers/tag_data.h"

#include "headers/tag_block.h"
#include "headers/tag_groups.h"
#include "headers/name_flags.h"
extern uint8_t file_exists(const file_reference *file);
extern void *file_read_into_memory(file_reference *reference, unsigned int *size);
extern char *file_reference_get_name(const file_reference *reference, unsigned int flags, char *name);
/* memcpy declared by <string.h> */

uint8_t hs_rebuild_source_file(file_reference *script_file)
{
    scenario *scn = global_scenario;
    if ( !file_exists(script_file) )
        return 0;

    int16_t element = tag_block_add_element(&scn->hs_source_files);
    if ( element == -1 )
        return 0;

    hs_source_file *entry = &((hs_source_file *)scn->hs_source_files.address)[element];
    unsigned int size;
    char name[304];

    const void *contents = file_read_into_memory(script_file, &size);
    if ( !contents || !tag_data_resize(&entry->source, size) )
        return 0;

    file_reference_get_name(script_file, (1u << _name_filename_bit), name);
    strncpy(entry->name, name, 0x1Fu);
    entry->name[31] = 0;

    void *pointer = tag_data_get_pointer(&entry->source, 0, size);
    memcpy(pointer, contents, size);
    return 1;
}
