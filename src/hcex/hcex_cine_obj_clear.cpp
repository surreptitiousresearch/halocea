/* hcex_cine_obj_clear @0x823B53E8 — abort any running cinematic on the ws-engine animation/cinematic system
 * and clear the HCEX cinematic-object name->index table.
 *
 * hcex-bridge code; the ws-engine cinematic system and container are declared as boundary externs. */

extern struct gsANITEC_SYS *gsSysAnitec;
extern void *hcexObjectsCine;  /* dsVECTOR<dsPAIR<dsTSTRING<char>,int>,8> */

extern void gsANITEC_SYS_AbortCinematic(struct gsANITEC_SYS *system);
extern void dsVECTOR_dsPAIR_dsTSTRING_char_int_8__Clear(void *vector);

void hcex_cine_obj_clear(void)
{
    gsANITEC_SYS_AbortCinematic(gsSysAnitec);
    dsVECTOR_dsPAIR_dsTSTRING_char_int_8__Clear(&hcexObjectsCine);
}
