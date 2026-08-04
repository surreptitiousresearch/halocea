/* ws-engine streaming boundary: strmVID_MEM_MANAGER is an opaque Saber class. */
typedef struct strmVID_MEM_MANAGER strmVID_MEM_MANAGER;

/* Global VID-memory manager instance (mangled: _strmMemMng__3VstrmVID_MEM_MANAGER__A). */
extern strmVID_MEM_MANAGER strmMemMng;

/* Method (mangled: strmVID_MEM_MANAGER::ReserveAllMemory(void)). */
void strmVID_MEM_MANAGER_ReserveAllMemory(strmVID_MEM_MANAGER *self);

/* Post-registration hook: reserve all streaming video memory once VBs are bound. */
extern "C" void hcex_after_register_vertex_buffers(void)
{
    strmVID_MEM_MANAGER_ReserveAllMemory(&strmMemMng);
}
