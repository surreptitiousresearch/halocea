/* main_save_core_name @0x83689258 — save a named debug checkpoint (thunk to the MCC checkpoint system). */

extern void hcex_save_dbg_checkpoint(const char *core_name);

void main_save_core_name(const char *core_name)
{
    hcex_save_dbg_checkpoint(core_name);
}
