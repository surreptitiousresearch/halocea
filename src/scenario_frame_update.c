/* scenario_frame_update @0x837033C8 — per-frame scenario update; a thunk straight to wind_update. */

extern void wind_update(void);

void scenario_frame_update(float dt)
{
    wind_update();
}
