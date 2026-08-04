#pragma once

/* cache_copy_buffer_sizes — cache-copy / simple-decompressor I/O buffer size constants.
 * DB-verified via types_enum_values $437A0D4145CF7F2706088D9E344D849E. */
enum cache_copy_buffer_sizes {
	FILE_BLOCK_SIZE = 0x20000,                 /* 131072 */
	WRITE_FILE_BLOCK_SIZE = 0x400000,          /* 4194304 */
	TOTAL_READ_WRITE_BUFFER_SIZE = 0x500000,   /* 5242880 */
	TOTAL_BUFFER_SIZE = 0x512000,              /* 5316608 */
};
