/* hcex_obj_term @0x823EE8F8 — tear down the ws-engine state for a Blam object id. Thunk that forwards
 * to hcex_obj_term_base(id, false) (the bool selects a non-forced teardown). */

extern void hcex_obj_term_base(int id, int forced);

extern "C" void hcex_obj_term(int id)
{
    hcex_obj_term_base(id, 0);
}
