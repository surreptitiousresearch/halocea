/* create_thread @0x83800050 — allocate a slot from the fixed thread pool and start a Win32 thread on it.
 * Scans thread_pool for the first free slot (returns 0 if the pool is full), spawns a suspended-capable
 * thread with a 16 KB stack, then applies a priority derived from the flags (bit1 -> lowest, bit2 -> above
 * normal, else normal) and resumes it. On any failure the handle is closed and 0 is returned. */

#include "headers/thread.h"
#include "headers/thread_attribute_flags.h"
#include "headers/blam_data_globals.h"

/* thread_pool @0x844A6208 holds 32 slots; mutex_pool @0x844A6308 immediately follows it, so the
 * decompiler read the pool's end sentinel as &mutex_pool[0]. Expressed here as &thread_pool[32]. */
#define THREAD_POOL_COUNT 32

extern void *CreateThread(void *security, unsigned int stack_size,
                          unsigned int (*start)(void *), void *param,
                          unsigned int flags, unsigned int *thread_id);
extern int   SetThreadPriority(void *handle, int priority);
extern int   ResumeThread(void *handle);
extern int   CloseHandle(void *handle);

int create_thread(
        unsigned __int16 flags,
        unsigned int (*function)(void *),
        void        *function_input,
        thread     **thread_reference)
{
    unsigned __int8 saved_flags = (unsigned __int8)flags;
    unsigned __int8 *p_in_use = &thread_pool[0].in_use;
    int slot = 0;
    unsigned int thread_id;

    while (*p_in_use)
    {
        p_in_use += 8; /* sizeof(thread) == 8 */
        ++slot;
        if ((char *)p_in_use >= (char *)&thread_pool[THREAD_POOL_COUNT])
            return 0;
    }

    thread *slot_thread = &thread_pool[slot];
    slot_thread->object = nullptr;
    slot_thread->in_use = 1;
    slot_thread->object = CreateThread(nullptr, 0x4000u, function, function_input, 4u, &thread_id);
    *thread_reference = slot_thread;
    if (slot_thread->object)
    {
        int priority;
        if ((saved_flags & (1u << _thread_attribute_flag_priority_low)) != 0)
            priority = -1;
        else
            priority = (saved_flags & (1u << _thread_attribute_flag_priority_high)) != 0;
        if (SetThreadPriority(slot_thread->object, priority) && ResumeThread(slot_thread->object) != -1)
            return 1;
        CloseHandle(slot_thread->object);
    }
    return 0;
}
