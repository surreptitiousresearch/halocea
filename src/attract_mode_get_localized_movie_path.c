/* attract_mode_get_localized_movie_path @0x83784D30 — resolves the on-disk path for one of the attract-mode
 * Bink movies (domain = enum bink_movie, DB $BC15C09D…; _bink_outro_movie's file is credits%s.bik — the
 * outro IS the credits movie, DB name and filename both authentic), preferring the
 * current system language's localized cut (suffix picked the same way as cache_files_map_directory's
 * "maps_xx\" selection: 3=de, 4=fr, 5=es, 6=it, 1/2=unsuffixed) and falling back through every other
 * language suffix in ascending order if that file doesn't exist. Returns an empty path if none of the 7
 * candidates exist. */

#include <stdint.h>
#include <stdio.h>

#include "headers/bink_movie.h"
#include "headers/file_reference.h"
#include "headers/xc_language.h"

extern const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */
extern const char *cache_files_root_directory(void);
extern int XTLGetLanguage(void);
extern int _snprintf_0(char *buffer, unsigned int size, const char *format, ...);
extern file_reference *file_reference_create_from_path(file_reference *reference, const char *path, uint8_t directory);
extern uint8_t file_exists(const file_reference *file);

static char path_1[128];

char *attract_mode_get_localized_movie_path(int16_t movie)
{
    const char *language_suffix[7];
    language_suffix[0] = "_de";
    language_suffix[1] = "_fr";
    language_suffix[2] = "_es";
    language_suffix[3] = "_it";
    language_suffix[4] = empty_string;
    language_suffix[5] = empty_string;
    language_suffix[6] = empty_string;

    int language = XTLGetLanguage();
    int suffix_index;
    if ((unsigned int)(language - 1) > 5)
    {
        suffix_index = 6;
    }
    else
    {
        switch (language)
        {
        case XC_LANGUAGE_ENGLISH:  suffix_index = 4; break;
        case XC_LANGUAGE_JAPANESE: suffix_index = 5; break;
        case XC_LANGUAGE_GERMAN:   suffix_index = 0; break;
        case XC_LANGUAGE_FRENCH:   suffix_index = 1; break;
        case XC_LANGUAGE_SPANISH:  suffix_index = 2; break;
        default: /* XC_LANGUAGE_ITALIAN */ suffix_index = 3; break;
        }
    }

    unsigned char tried_mask = 0;
    for (;;)
    {
        const char *root = cache_files_root_directory();
        const char *suffix = language_suffix[suffix_index];

        switch (movie)
        {
        case _bink_attract1_movie:
            _snprintf_0(path_1, 0x80u, "%sbink\\attract1%s.bik", root, suffix);
            break;
        case _bink_attract2_movie:
            _snprintf_0(path_1, 0x80u, "%sbink\\attract2%s.bik", root, suffix);
            break;
        case _bink_attract3_movie:
            _snprintf_0(path_1, 0x80u, "%sbink\\attract3%s.bik", root, suffix);
            break;
        case _bink_intro_movie:
            _snprintf_0(path_1, 0x80u, "%sbink\\intro%s.bik", root, suffix);
            break;
        case _bink_outro_movie:
            _snprintf_0(path_1, 0x80u, "%sbink\\credits%s.bik", root, suffix);
            break;
        default:
            _snprintf_0(path_1, 0x80u, "%sbink\\teaser%s.bik", root, suffix);
            break;
        }

        file_reference reference;
        if (file_exists(file_reference_create_from_path(&reference, path_1, 0)))
            break;

        tried_mask |= (unsigned char)(1 << suffix_index);
        int next_index = 0;
        while ((tried_mask & (1 << next_index)) != 0 && next_index < 7)
            ++next_index;
        suffix_index = next_index;

        if (suffix_index == 7)
        {
            path_1[0] = 0;
            return path_1;
        }
    }
    return path_1;
}
