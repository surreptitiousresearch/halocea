#pragma once
/* datastore_entry — one record of a Blam datastore file (the save/settings store read and written
 * by datastore_read / datastore_write). Fixed 510-byte slot: a 255-byte name followed by a
 * 255-byte payload, both un-padded, which is why the stride is odd.
 * Layout verbatim from the database (types_members datastore_entry, size 510). */

typedef struct datastore_entry
{
    char name[255]; /* 0x000 */
    char data[255]; /* 0x0FF */
} datastore_entry;  /* 0x1FE */
