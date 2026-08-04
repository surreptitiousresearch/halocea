extern void random_math_initialize(void);
extern void periodic_functions_initialize(void);

/* attested void: no r3 written (disasm blr @0x836FC120, last op is the periodic_functions_initialize
   call); the decompiler's `return 0` was r3-scratch. Sole caller shell_initialize ignores it. */
void real_math_initialize(void)
{
    random_math_initialize();
    periodic_functions_initialize();
}
