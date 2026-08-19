/* input_abstraction_test_config_one_control @0x83724F38 — config-mode test of a single
 * control (no-op in this retail build). */

/* const-qualified 2026-07-30: arg is an hs string-pool pointer; no-op body writes nothing (C4090) */
void input_abstraction_test_config_one_control(const char *control_specifier)
{
}
