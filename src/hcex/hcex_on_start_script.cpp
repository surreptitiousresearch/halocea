/* hcex_on_start_script @0x823E4928 — HCEX bridge hook fired when a Blam scripted event named `script_name`
 * begins. It matches the name against a fixed set of scripted checkpoints and fires the matching Halo:CE
 * achievement check (and, for the finale ride, a player event). The string comparisons are the compiler's
 * inlined strcmp; expressed here as strcmp() calls.
 *
 * The hcex_check_ach* helpers and hcex_fire_plr_event are hcex-bridge / Blam siblings (extern boundaries). */

extern int  strcmp(const char *a, const char *b);

extern void hcex_check_ach22(void);
extern void hcex_check_ach31(void);
extern void hcex_check_ach37(void);
extern void hcex_check_ach24(void);
extern void hcex_check_ach34(void);
extern void hcex_fire_plr_event(const char *event_name, int plr_idx);

extern "C" void hcex_on_start_script(const char *script_name)
{
    if ( !script_name )
        return;

    if ( strcmp(script_name, "enc_area2_wave_2") == 0 )
    {
        hcex_check_ach22();
    }
    else if ( strcmp(script_name, "cutscene_extraction") == 0 )
    {
        hcex_check_ach31();
    }
    else if ( strcmp(script_name, "cinematic_finale") == 0 )
    {
        hcex_check_ach37();
        hcex_fire_plr_event("complete_ride_d40", -1);
    }
    else if ( strcmp(script_name, "save_beach_lz") == 0 )
    {
        hcex_check_ach24();
    }
    else if ( strcmp(script_name, "cinematic_outro") == 0 )
    {
        hcex_check_ach34();
    }
}
