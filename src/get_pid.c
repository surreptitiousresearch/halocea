/* get_pid @0x83804010 — returns a fixed placeholder format-template string ("xxxx dddd dddd dddd dddd"),
 * not an actual process id (confirmed via disasm: address-of a string literal, no computation). */

const char * get_pid(void)
{
    return "xxxx dddd dddd dddd dddd";
}
