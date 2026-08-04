/* cache_files_map_directory @0x836F6D68 — pick the map subdirectory for the current system language
 * (localized "maps_xx\" variants, falling back to "maps\"), then confirm it exists under the cache root or any
 * alternative directory. Returns the chosen relative directory string. */

#include <stdint.h>
#include "headers/file_reference.h"
#include "headers/xc_language.h"
#include "headers/blam_data_globals.h"


extern int   XTLGetLanguage(void);
extern char *strncpy(char *dst, const char *src, unsigned int n);
extern char *strncat(char *dst, const char *src, unsigned int n);
extern file_reference *file_reference_create_from_path(file_reference *reference, const char *path, uint8_t directory);
extern uint8_t file_exists(const file_reference *file);

static int directory_path_exists(const char *cache_root, const char *subdir)
{
    char path[256];
    strncpy(path, cache_root, 0x100u);

    char *end = path;
    while ( *end++ )
        ;
    strncat(path, subdir, 256 - (end - path - 1));

    file_reference reference;
    return file_exists(file_reference_create_from_path(&reference, path, 1u));
}

char *cache_files_map_directory(void)
{
    int language = XTLGetLanguage();
    const char *map_dir;
    if ( language < XC_LANGUAGE_GERMAN || language > XC_LANGUAGE_ITALIAN )
    {
        map_dir = "maps\\";
    }
    else
    {
        switch ( language )
        {
            case XC_LANGUAGE_GERMAN:  map_dir = "maps_de\\"; break;
            case XC_LANGUAGE_FRENCH:  map_dir = "maps_fr\\"; break;
            case XC_LANGUAGE_SPANISH: map_dir = "maps_es\\"; break;
            default: /* XC_LANGUAGE_ITALIAN */ map_dir = "maps_it\\"; break;
        }
    }

    if ( directory_path_exists(cache_root_directory, map_dir) )
        return (char *)map_dir;

    if ( !alternative_directories[0] )
        return (char *)map_dir;

    for ( int i = 0; alternative_directories[i]; ++i )
    {
        if ( directory_path_exists(cache_root_directory, alternative_directories[i]) )
            return alternative_directories[i];
    }
    return (char *)map_dir;
}
