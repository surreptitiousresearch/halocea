/* ws-engine platform-abstraction boundary global (mangled: _apHInstance__3PAXA). */
extern void *apHInstance;

/* Getter: returns the process HINSTANCE the ap-layer cached at startup. */
extern "C" void *hcex_get_instance(void)
{
    return apHInstance;
}
