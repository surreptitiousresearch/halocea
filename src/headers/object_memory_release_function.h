#pragma once
/* object_memory_release_function — one entry of the object_memory_release_procs[3] table. The garbage
 * collector, when it cannot free enough object-pool space by deleting invisible garbage objects, walks
 * this table asking each subsystem to release objects it owns. Layout from the database. */

typedef struct object_memory_release_function
{
    void          (*init_function)(unsigned char *buffer, __int16 buffer_size);
    unsigned __int8 (*function)(char *description, unsigned __int8 *more, unsigned char *buffer, __int16 buffer_size);
} object_memory_release_function;
