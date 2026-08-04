#pragma once

/* Standard reflected CRC-32 (polynomial 0xEDB88320) lookup table, built
   lazily on first use by crc_checksum_buffer. */
extern unsigned int crc_table_1[256];
extern unsigned char crc_table_built;
