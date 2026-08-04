/* hcex_pix_end_event @ 0x823CED88
   Closes the current PIX profiler event, but only when PIX instrumentation is
   enabled. osPIXEndEvent is a ws-engine profiling boundary. */

extern unsigned char hcex_enable_pix;   /* bool hcex_enable_pix */
extern void osPIXEndEvent(void);

extern "C" void hcex_pix_end_event(void)
{
    if ( hcex_enable_pix )
        osPIXEndEvent();
}
