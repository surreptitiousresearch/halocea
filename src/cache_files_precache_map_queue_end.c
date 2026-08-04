/* cache_files_precache_map_queue_end @0x83753E60 — thunk to cache_copy_queue_end. */
extern void cache_copy_queue_end(void);
void cache_files_precache_map_queue_end(void) /* attested void: tail-b of void fn, 0/1 callers consume r3 */
{
    cache_copy_queue_end();
}
