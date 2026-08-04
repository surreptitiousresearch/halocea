/* hcex_dump_mdl_name @0x823CE368 — debug helper: convert a model tag path to its HCEX key and print it.
 * Both callees are HCEX siblings. */

extern char *hcex_conv_mdl_name(const char *mdl_name, char *out, int max_len);
extern void hcex_output_dbg(const char *format, ...);

extern "C" void hcex_dump_mdl_name(const char *mdl_name)
{
    char converted_name[72];

    hcex_conv_mdl_name(mdl_name, converted_name, 64);
    hcex_output_dbg("%s\n", converted_name);
}
