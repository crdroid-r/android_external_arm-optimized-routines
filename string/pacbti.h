/*
 * Macros for pacbti asm code.
 *
 * Copyright (c) 2022, Arm Limited.
 * SPDX-License-Identifier: MIT OR Apache-2.0 WITH LLVM-exception
 */

/* Checki whether leaf function PAC signing has been requested
   in the -mbranch-protect compile-time option */
#define LEAF_PROTECT_BIT 2
#define HAVE_PAC_LEAF \
	__ARM_FEATURE_PAC_DEFAULT & (1 << LEAF_PROTECT_BIT)

/* Macro to handle function entry depending on branch-protection
   schemes */
	.macro pacbti_prologue
#if HAVE_PAC_LEAF
#if __ARM_FEATURE_BTI_DEFAULT
	pacbti ip, lr, sp
#else
	pac ip, lr, sp
#endif /* __ARM_FEATURE_BTI_DEFAULT */
	.cfi_register 143, 12
	str ip, [sp, #-4]!
	.save {ra_auth_code}
	.cfi_def_cfa_offset 4
	.cfi_offset 143, -4
#elif __ARM_FEATURE_BTI_DEFAULT
	bti
#endif /* HAVE_PAC_LEAF */
	.endm

/* Macro to handle different branch exchange cases depending on
   branch-protection schemes */
	.macro pacbti_epilogue
#if HAVE_PAC_LEAF
	ldr ip, [sp], #4
	.cfi_restore 143
	.cfi_def_cfa_offset 0
	aut ip, lr, sp
#endif /* HAVE_PAC_LEAF */
	bx lr
	.endm
