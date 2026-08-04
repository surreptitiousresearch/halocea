#pragma once
/* file_last_modification_date — opaque 8-byte timestamp (a Win32 FILETIME) copied out of a directory
 * entry's last-write time. Layout from the database (8 bytes). */

typedef struct file_last_modification_date
{
    char data[8];
} file_last_modification_date;
