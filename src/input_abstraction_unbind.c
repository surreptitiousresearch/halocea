/* input_abstraction_unbind @0x83724F50 — remove a device-button binding
 * (no-op in this retail build). */

/* const-qualified 2026-07-30: args are hs string-pool pointers; no-op body writes nothing (C4090) */
void input_abstraction_unbind(const char *device_specifier, const char *button_specifier)
{
}
