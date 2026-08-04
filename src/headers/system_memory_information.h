#pragma once
/* system_memory_information — free/total physical memory in bytes, DB layout (8 bytes). */

typedef struct system_memory_information
{
    int free;  /* 0x0 */
    int total; /* 0x4 */
} system_memory_information; /* 8 bytes */
