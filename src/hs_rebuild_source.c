/* hs_rebuild_source @0x8372F9D8 — (dev/tool path) rebuild the scenario's HaloScript source-file list from
 * disk: clear the existing list, then compile global_scripts.hsc (skipped for the UI scenario) and every
 * "*.hsc" file under ..\source\halo\<scenario_dir>\scripts (sorted). Returns 1 if all files compiled.
 *
 * Deviations: the two inline byte-compare loops are strcmp (scenario name vs "levels\ui\ui", and the file
 * extension vs "hsc"); each find_files element is 0x10C bytes (a file_reference). tag_block_resize is a
 * no-op in the cache build, so this path is effectively inert there. */

#include <stdint.h>
#include "headers/file_reference.h"
#include "headers/scenario.h"
#include <string.h>

#include "headers/tag_block.h"
extern uint8_t tag_block_resize(tag_block *block, int element_count);
extern char *tag_get_name(int16_t tag_index);
extern int sprintf_0(char *string, const char *format, ...);
extern file_reference *file_reference_create_from_path(file_reference *reference, const char *path, uint8_t directory);
extern uint8_t file_exists(const file_reference *file);
extern uint8_t hs_rebuild_source_file(file_reference *script_file);
extern int16_t find_files(unsigned int flags, const file_reference *directory, int maximum_count, file_reference *references);
extern char *file_reference_get_name(const file_reference *reference, unsigned int flags, char *name);
extern void qsort(void *base, unsigned int count, unsigned int size, int (__fastcall *compare)(const void *, const void *));
extern int alphabetize_file_references(const file_reference *a, const file_reference *b);

uint8_t hs_rebuild_source(void)
{
    int success = 1;
    tag_block_resize(&global_scenario->hs_source_files, 0);

    char scripts_dir[256];
    sprintf_0(scripts_dir, "..\\source\\halo\\%s", tag_get_name(global_scenario_index));
    char *leaf = strrchr(scripts_dir, '\\');
    sprintf_0(leaf + 1, "scripts");

    file_reference global_scripts;
    file_reference_create_from_path(&global_scripts, "..\\source\\halo\\global_scripts.hsc", 0);
    if ( file_exists(&global_scripts) )
    {
        /* the UI scenario does not include global_scripts.hsc */
        if ( strcmp(tag_get_name(global_scenario_index), "levels\\ui\\ui") != 0 )
            success = hs_rebuild_source_file(&global_scripts);
    }

    file_reference directory;
    file_reference_create_from_path(&directory, scripts_dir, 1u);

    file_reference found[8];
    int file_count = find_files(0, &directory, 8, found);
    /* standard qsort idiom: typed comparator cast to qsort's generic const void* signature */
    qsort(found, file_count, 0x10Cu, (int (__fastcall *)(const void *, const void *))alphabetize_file_references);

    for ( __int16 i = 0; i < file_count; i = (__int16)(i + 1) )
    {
        char name[256];
        file_reference_get_name(&found[i], 8u, name);
        if ( strcmp(name, "hsc") == 0 )
        {
            if ( hs_rebuild_source_file(&found[i]) == 0 )
                success = 0;
        }
    }

    return success;
}
