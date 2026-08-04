extern void random_math_dispose(void);
extern void periodic_functions_dispose(void);

/* real_math_dispose disposes the two math subsystems and returns nothing meaningful (disasm 0x836FC128:
 * void tail call, r3 dead); retyped void and dropped the spurious decompiler-invented uninitialized
 * `random_dispose_result` arg (periodic_functions_dispose takes none). 2026-07-31 */
void real_math_dispose(void)
{
    random_math_dispose();
    periodic_functions_dispose();
}
