/* hcex_check_ach44 @0x823E4AC8 — fire the ACH44 player event once the player has collected at least
 * two gold skulls and one silver skull. */

extern int hcex_gold_skulls_count;
extern int hcex_silver_skulls_count;

extern void hcex_fire_plr_event(const char *event_name, short player_identifier);

extern "C" void hcex_check_ach44(void)
{
    if ( hcex_gold_skulls_count >= 2 && hcex_silver_skulls_count >= 1 )
        hcex_fire_plr_event("ACH44", -1);
}
