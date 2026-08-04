/* file_get_last_modification_date @0x837E6F8C — resolve the file_reference to a full path and read its last
 * write time via GetFileAttributesExA. Zeroes *date defensively before the call and leaves it zeroed if the
 * attributes query fails. Always returns 1 (the success/failure is only used to decide whether to clear the
 * last error, not reflected in the return value) — reproduced faithfully.
 *
 * Deviation: the decompiler split GetFileAttributesExA's WIN32_FILE_ATTRIBUTE_DATA output buffer into two
 * adjacent stack locals (a 20-byte blob immediately followed by an 8-byte `file_last_modification_date`) because
 * only the trailing ftLastWriteTime field is read back out. Reassembled as a single real
 * WIN32_FILE_ATTRIBUTE_DATA local, which is what GetFileExInfoStandard actually expects. */

#include <stdint.h>
#include <windows.h>
#include <string.h>
#include "headers/file_reference.h"
#include "headers/file_last_modification_date.h"

extern void file_location_get_full_path(int16_t location, const char *path, char *full_path);

uint8_t file_get_last_modification_date(const file_reference *file, file_last_modification_date *date)
{
    char full_path[288];
    WIN32_FILE_ATTRIBUTE_DATA attribute_data;
    unsigned __int8 succeeded = 0;

    memset(date, 0, sizeof(*date));
    memset(full_path, 0, 256);
    file_location_get_full_path(*(unsigned short *)&file->data[6], &file->data[8], full_path);

    if ( GetFileAttributesExA(full_path, GetFileExInfoStandard, &attribute_data) )
    {
        succeeded = 1;
        /* recovered: *(file_last_modification_date *)&ftLastWriteTime -> copy into date->data */
        memcpy(date->data, &attribute_data.ftLastWriteTime, sizeof(date->data));
    }

    if ( !succeeded )
    {
        GetLastError();
        SetLastError(0);
    }

    return 1;
}
