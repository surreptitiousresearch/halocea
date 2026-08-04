/* hcex_correct_time_elapsed @ 0x823CE770
   Overrides a caller-supplied frame elapsed-time value with the ws-engine's
   corrected elapsed time, but only when the engine has modified it this frame.
   gsElapsedTime / gsElapsedTimeModified are ws-engine timing globals. */

extern float gsElapsedTime;
extern int gsElapsedTimeModified;

extern "C" void hcex_correct_time_elapsed(float *time)
{
    if ( gsElapsedTimeModified )
        *time = gsElapsedTime;
}
