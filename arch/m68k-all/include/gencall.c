/*
    Copyright © 1995-2001, The AROS Development Team. All rights reserved.
    $Id$
*/
/* This program generates the libcall.h macroset for gcc-4.5.1 m68k-elf
 *
 * WARNING: The header generated by this program is designed
 *          to work with GCC 4.5.1 m68k-elf ONLY, and even
 *          then it has only been very lightly tested with
 *          "-O2 -fomit-frame-pointer -ffixed-a6"
 *
 * If it breaks, you get to keep both pieces.
 */

#include <stdio.h>

#define GENCALL_MAX	(13 + 1)	/* Max number of arguments */

/* NOTE: For all 'call' macros, ie AROS_LC5(), the
 *       'bt' parameter is frequently garbled by
 *       callers who use '#define's like:
 *
 * #define UtilityBase mydata->utilitybase
 *
 *       this makes a 'bt' parameter of 'struct UtilityBase *'
 *       turn into 'struct mydata->utilitybase *'. 
 *
 *       Unhappiness ensues. So, we must use 'void *'
 *       for all 'bt' arguments in the call macros.
 */
void aros_ufp(int id, int is_static)
{
	int i;

	printf("#define AROS_UFP%d%s(t,n", id, is_static ? "S" : "");
	for (i = 0; i < id; i++)
		printf(",a%d", i + 1);
	printf(") \\\n");
	printf("\t%st n (void)\n", is_static ? "static " : "");
}

void aros_ufh(int id, int is_static)
{
	int i;

	printf("#define AROS_UFH%d%s(t,n", id, is_static ? "S" : "");
	for (i = 0; i < id; i++)
		printf(",a%d", i + 1);
	printf(") \\\n");
	printf("\t%st n (void) {%s\n", is_static ? "static " : "", (i==0) ? "" : " \\");
	for (i = 0; i < id; i++)
		printf(" \\\n\t__AROS_UFPA(a%d) __attribute__((unused)) __AROS_UFCA(a%d) = ({register ULONG __r asm(__AROS_LSA(a%d));(__AROS_UFPA(a%d))__r;});", i+1, i+1, i+1, i+1);
	printf("\n");
}

static void asm_regs_init(int id, int has_bn)
{
    int i;

    /* Input values */
    for (i = 0; i < id; i++)
	printf("\t   ULONG _arg%d = (ULONG)__AROS_LCA(a%d); \\\n",
		i + 1, i + 1);
    if (has_bn)
    	printf("\t   ULONG _bn_arg = (ULONG)bn; \\\n");

    printf("\t    asm volatile (\"move.l %%a5,%%sp@(-4)\\n\"); \\\n");

    /* Define registers */
    printf("\t   register volatile ULONG _ret asm(\"%%d0\"); \\\n");
    if (has_bn)
    	printf("\t   register volatile ULONG _bn asm(\"%%a6\") = _bn_arg; \\\n");
    for (i = 0; i < id; i++)
	printf("\t   register volatile ULONG __AROS_LTA(a%d) asm(__AROS_LSA(a%d)); \\\n",
		i + 1, i + 1);

    /* Set registers */
    for (i = 0; i < id; i++)
	printf("\t   __AROS_LTA(a%d) = _arg%d; \\\n",
		i + 1, i + 1);
    if (has_bn)
    	printf("\t   _bn = _bn_arg; \\\n");

    printf("\t    asm volatile (\"subq.l #4,%%%%sp\\n\" \\\n");
}

static void asm_regs_exit(int id, int has_bn)
{
    printf("\t    asm volatile (\"move.l %%sp@+,%%a5\\n\"); \\\n");

    /* Save retval */
    printf("\t  (t)_ret; \\\n");
}

static void aros_ufc(int id)
{
	int i;
	printf("#define AROS_UFC%d(t,n", id);
	for (i = 0; i < id; i++)
		printf(",a%d", i + 1);
	printf(") \\\n");
	printf("\t({ APTR _n = (n);\\\n");
	asm_regs_init(i, 0);
	printf("\t\t\"pea.l .Lufc%d_%%c1\\n\" \\\n", id);
	printf("\t\t\"move.l %%0, %%%%sp@-\\n\" \\\n");
	printf("\t\t: : \\\n");
	printf("\t\t  \"r\" (_n), \"i\" (__LINE__) \\\n");
	for (i = 0; i < id; i++)
		printf("\t\t, \"r\" (__AROS_LTA(a%d)) \\\n", i + 1);
	printf("\t\t); \\\n");
	printf("\t   asm volatile (\"rts\\n.Lufc%d_%%c0:\\n\" : : \"i\" (__LINE__) : ); \\\n", id);
	printf("\t   asm volatile (\"\" : \"=r\" (_ret) : : \"%%a0\", \"%%a1\", \"%%d1\", \"cc\", \"memory\"); \\\n");
	asm_regs_exit(i, 0);
	printf("\t  })\n\n");
}

void aros_lc(int id)
{
	int i;
	printf("#define AROS_LC%d(t,n,", id);
	for (i = 0; i < id; i++)
		printf("a%d,", i + 1);
	printf("bt,bn,o,s) \\\n");
	printf("\t({ \\\n");
	asm_regs_init(id, 1);
	printf("\t\t\"jsr %%c0(%%%%a6)\" \\\n");
	printf("\t\t: : \\\n");
	printf("\t\t  \"i\" (-1 * (o) * LIB_VECTSIZE) \\\n");
	for (i = 0; i < id; i++)
		printf("\t\t, \"r\" (__AROS_LTA(a%d)) \\\n", i + 1);
	printf("\t\t, \"r\" (_bn)); \\\n");
	printf("\t   asm volatile (\"\" : \"=r\" (_ret) : : \"%%a0\", \"%%a1\", \"%%d1\", \"cc\", \"memory\"); \\\n");
	asm_regs_exit(id, 1);
	printf("\t  })\n\n");
}

void aros_lh(int id, int is_ignored)
{
	int i;

	printf("#define AROS_LH%d%s(t,n,", id, is_ignored ? "I" : "");
	for (i = 0; i < id; i++)
		printf("a%d,", i + 1);
	printf("bt,bn,o,s) \\\n");
	printf("\tt AROS_SLIB_ENTRY(n,s) (void) {");
	for (i = 0; i < id; i++)
		printf(" \\\n\t__AROS_LPA(a%d) __attribute__((unused)) __AROS_LCA(a%d) = ({register ULONG __r asm(__AROS_LSA(a%d));(__AROS_LPA(a%d))__r;});", i+1, i+1, i+1, i+1);
	if (!is_ignored)
		printf(" \\\n\tregister bt __attribute__((unused)) bn = ({register ULONG __r asm(\"%%a6\");(bt)__r;});");
	printf("\n");
}

static void aros_lcnr(int id)
{
	printf("#define AROS_LC%dNR AROS_LC%d\n", id, id);
}

static void aros_call(int id)
{
	int i;
	printf("#define AROS_CALL%d(t,n,", id);
	for (i = 0; i < id; i++)
		printf("a%d,", i + 1);
	printf("bt,bn) \\\n");
	printf("\tAROS_UFC%d(t,n", id + 1);
	for (i = 0; i < id; i++) {
		printf(",AROS_UFCA(a%d)", i + 1);
	}
	printf(",AROS_UFCA(bt,bn,A6))\n");
}

static void aros_callnr(int id)
{
	printf("#define AROS_CALL%dNR AROS_CALL%d\n", id, id);
}

static void aros_lvo_call(int id)
{
	int i;
	printf("#define AROS_LVO_CALL%d(t,", id);
	for (i = 0; i < id; i++)
		printf("a%d,", i + 1);
	printf("bt,bn,o,s) \\\n");
	printf("\tAROS_CALL%d(t,__AROS_GETVECADDR(bn,o), \\\n", id);
	for (i = 0; i < id; i++)
		printf("\t\tAROS_LCA(a%d), \\\n", i + 1);
	printf("\t\tbt,bn)\n");
}

static void aros_lvo_callnr(int id)
{
	int i;
	printf("#define AROS_LVO_CALL%dNR(t,", id);
	for (i = 0; i < id; i++)
		printf("a%d,", i + 1);
	printf("bt,bn,o,s) \\\n");
	printf("\tAROS_CALL%dNR(t,__AROS_GETVECADDR(bn,o), \\\n", id);
	for (i = 0; i < id; i++)
		printf("\t\tAROS_LCA(a%d), \\\n", i + 1);
	printf("\t\tbt,bn)\n");
}

static void aros_ld(int id, int is_ignored)
{
	int i;

	printf("#define AROS_LD%d%s(t,n,", id, is_ignored ? "I" : "");
	for (i = 0; i < id; i++)
		printf("a%d,", i + 1);
	printf("bt,bn,o,s) \\\n");
	printf("\t__AROS_LD_PREFIX t AROS_SLIB_ENTRY(n,s) (void)\n");
}

int main(int argc, char **argv)
{
	int i;

	printf("/* AUTOGENERATED by arch/m68k-all/include/gencall.c */\n");
	printf("/* If you can get this to work for anything other   */\n");
	printf("/* than gcc-4.5.1 m68k-elf, it would be surprising. */\n");
	printf("\n");
	printf("#ifndef AROS_M68K_LIBCALL_H\n");
	printf("#define AROS_M68K_LIBCALL_H\n");
	printf("\n");
	printf("/* Call a libary function which requires the libbase */\n");
	printf("\n");
	printf("#define __AROS_CPU_SPECIFIC_ASMCALLS\n\n");

	for (i = 0; i < GENCALL_MAX; i++)
		aros_ufp(i, 0);

	for (i = 0; i < GENCALL_MAX; i++)
		aros_ufp(i, 1);

	for (i = 0; i < GENCALL_MAX; i++)
		aros_ufh(i, 0);

	for (i = 0; i < GENCALL_MAX; i++)
		aros_ufh(i, 1);

	for (i = 0; i < GENCALL_MAX; i++)
		aros_ufc(i);

	printf("#define __AROS_CPU_SPECIFIC_LH\n\n");
	
	for (i = 0; i < GENCALL_MAX; i++)
		aros_lh(i, 0);

	for (i = 0; i < GENCALL_MAX; i++)
		aros_lh(i, 1);

	printf("\n");
	printf("#define __AROS_CPU_SPECIFIC_LC\n\n");
	
	for (i = 0; i < GENCALL_MAX; i++)
		aros_lc(i);

	for (i = 0; i < GENCALL_MAX; i++)
		aros_lcnr(i);

	for (i = 0; i < GENCALL_MAX; i++)
		aros_call(i);

	for (i = 0; i < GENCALL_MAX; i++)
		aros_callnr(i);

	for (i = 0; i < GENCALL_MAX; i++)
		aros_lvo_call(i);

	for (i = 0; i < GENCALL_MAX; i++)
		aros_lvo_callnr(i);

	printf("\n");
	printf("#define __AROS_CPU_SPECIFIC_LD\n\n");
	
	for (i = 0; i < GENCALL_MAX; i++)
		aros_ld(i, 0);

	for (i = 0; i < GENCALL_MAX; i++)
		aros_ld(i, 1);

	printf("#endif /* AROS_M68K_LIBCALL_H */\n");
	return 0;
}
