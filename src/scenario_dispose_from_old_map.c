/* scenario_dispose_from_old_map @ 0x837033C0 — thunk to wind_dispose_from_old_map */
/* attested: void return — tail call to void callee; sole caller (game_dispose_from_old_map) ignores r3 */
extern void wind_dispose_from_old_map(void);
void scenario_dispose_from_old_map(void)
{
    wind_dispose_from_old_map();
}
