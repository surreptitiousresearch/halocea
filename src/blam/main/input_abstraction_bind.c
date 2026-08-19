/* input_abstraction_bind @0x83724F48 — bind a device button to an abstract control
 * (no-op in this retail build; the bind table is fixed). */

/* const-qualified 2026-07-30: args are hs string-pool pointers; no-op body writes nothing (C4090) */
void input_abstraction_bind(const char *device_specifier, const char *button_specifier, const char *control_specifier)
{
}
