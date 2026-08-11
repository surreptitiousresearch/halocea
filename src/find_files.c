/* find_files @0x83767B80 — enumerate up to maximum_count directory entries matching `flags`, filling
 * the references array; returns the number found. */

#include <stdint.h>
#include "headers/file_reference.h"

#include "headers/file_last_modification_date.h"
extern void find_files_start(unsigned int flags, const file_reference *directory);
extern uint8_t find_files_next(file_reference *file, file_last_modification_date *date);

int16_t find_files(unsigned int flags, const file_reference *directory, int maximum_count, file_reference *references)
{
    int count = 0;
    find_files_start(flags, directory);
    if ( maximum_count > 0 )
    {
        do
        {
            if ( !find_files_next(references, 0) )
                break;
            ++count;
            ++references;
        }
        while ( count < maximum_count );
    }
    return count;
}
