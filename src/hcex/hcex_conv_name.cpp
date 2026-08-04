/* hcex_conv_name @ 0x823F2478
   Sanitizes a texture-manager key in place: replaces '\', ' ', '~', '-' with
   '_'. If the resulting length reaches max_len it triggers a hard fault
   (null-store) as an overflow assert.

   Deviation: the binary's overflow guard is a write to address 0 (MEMORY[0]=1);
   reproduced as a deliberate null store with this note. */

extern "C" char *hcex_conv_name(char *out, int max_len)
{
    char *cursor = out;
    if ( out )
    {
        if ( *out )
        {
            do
            {
                int ch = *cursor;
                if ( ch == '\\' || ch == ' ' || ch == '~' || ch == '-' )
                    *cursor = '_';
                ++cursor;
            }
            while ( *cursor );
        }
        if ( cursor - out >= max_len )
            *(volatile char *)0 = 1; /* overflow assert: MEMORY[0] = 1 */
    }
    return out;
}
