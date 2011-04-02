	.text
.globl _get_time
_get_time:
LFB7:
	pushq	%rbp
LCFI0:
	movq	%rsp, %rbp
LCFI1:
	subq	$48, %rsp
LCFI2:
	leaq	-16(%rbp), %rsi
	leaq	-32(%rbp), %rdi
	call	_gettimeofday
	movq	-32(%rbp), %rax
	cvtsi2sdq	%rax, %xmm2
	movl	-24(%rbp), %eax
	cvtsi2sd	%eax, %xmm1
	movsd	LC0(%rip), %xmm0
	mulsd	%xmm1, %xmm0
	movapd	%xmm2, %xmm1
	addsd	%xmm0, %xmm1
	movsd	%xmm1, -40(%rbp)
	fldl	-40(%rbp)
	leave
	ret
LFE7:
.globl _factorial_recursive
_factorial_recursive:
LFB8:
	pushq	%rbp
LCFI3:
	movq	%rsp, %rbp
LCFI4:
	subq	$16, %rsp
LCFI5:
	movq	%rdi, -8(%rbp)
	cmpq	$0, -8(%rbp)
	jne	L4
	movq	$1, -16(%rbp)
	jmp	L6
L4:
	movq	-8(%rbp), %rdi
	decq	%rdi
	call	_factorial_recursive
	movq	%rax, %rdx
	imulq	-8(%rbp), %rdx
	movq	%rdx, -16(%rbp)
L6:
	movq	-16(%rbp), %rax
	leave
	ret
LFE8:
.globl _factorial_while
_factorial_while:
LFB9:
	pushq	%rbp
LCFI6:
	movq	%rsp, %rbp
LCFI7:
	movq	%rdi, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, -8(%rbp)
	cmpq	$0, -24(%rbp)
	jne	L12
	movq	$1, -32(%rbp)
	jmp	L11
L13:
	decq	-24(%rbp)
	movq	-8(%rbp), %rax
	imulq	-24(%rbp), %rax
	movq	%rax, -8(%rbp)
L12:
	cmpq	$1, -24(%rbp)
	ja	L13
	movq	-8(%rbp), %rax
	movq	%rax, -32(%rbp)
L11:
	movq	-32(%rbp), %rax
	leave
	ret
LFE9:
.globl _factorial_asm
_factorial_asm:
LFB10:
	pushq	%rbp
LCFI8:
	movq	%rsp, %rbp
LCFI9:
	movq	%rdi, -24(%rbp)
	movq	-24(%rbp), %rax
	testq	%rax, %rax
	jne	L17
	movq	$1, -32(%rbp)
	jmp	L19
L17:
	    movq -24(%rbp), %rax 
	    movq -24(%rbp), %rcx 
	    decq       %rcx 
	FACTORIAL:    mulq %rcx 
	    decq %rcx 
	    jnz FACTORIAL 
	    movq %rax, -8(%rbp)
	movq	-8(%rbp), %rax
	movq	%rax, -32(%rbp)
L19:
	movq	-32(%rbp), %rax
	leave
	ret
LFE10:
.globl _factorial_for
_factorial_for:
LFB11:
	pushq	%rbp
LCFI10:
	movq	%rsp, %rbp
LCFI11:
	movq	%rdi, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, -8(%rbp)
	cmpq	$0, -24(%rbp)
	jne	L25
	movq	$1, -32(%rbp)
	jmp	L24
L26:
	decq	-24(%rbp)
	movq	-8(%rbp), %rax
	imulq	-24(%rbp), %rax
	movq	%rax, -8(%rbp)
L25:
	cmpq	$1, -24(%rbp)
	ja	L26
	movq	-8(%rbp), %rax
	movq	%rax, -32(%rbp)
L24:
	movq	-32(%rbp), %rax
	leave
	ret
LFE11:
	.cstring
	.align 3
LC2:
	.ascii "\12---------------\12Evaluating 50!:\12---------------\12Recursive: %lu - %.20Lf\12While:     %lu - %.20Lf\12Assembler: %lu - %.20Lf\12For:       %lu - %.20Lf\12\12\0"
	.text
.globl _main
_main:
LFB12:
	pushq	%rbp
LCFI12:
	movq	%rsp, %rbp
LCFI13:
	subq	$208, %rsp
LCFI14:
	call	_get_time
	fstp	%st(0)
	movl	$0, %eax
	movl	$0, %edx
	movq	%rax, -64(%rbp)
	movl	%edx, -56(%rbp)
	movl	$0, %eax
	movl	$0, %edx
	movq	%rax, -80(%rbp)
	movl	%edx, -72(%rbp)
	movl	$0, %eax
	movl	$0, %edx
	movq	%rax, -96(%rbp)
	movl	%edx, -88(%rbp)
	movl	$0, %eax
	movl	$0, %edx
	movq	%rax, -112(%rbp)
	movl	%edx, -104(%rbp)
	movq	$0, -8(%rbp)
	jmp	L30
L31:
	call	_get_time
	fstpt	-128(%rbp)
	movl	$50, %edi
	call	_factorial_recursive
	movq	%rax, -16(%rbp)
	call	_get_time
	fstpt	-144(%rbp)
	fldt	-144(%rbp)
	fldt	-128(%rbp)
	fsubrp	%st, %st(1)
	fldt	-64(%rbp)
	faddp	%st, %st(1)
	fstpt	-64(%rbp)
	incq	-8(%rbp)
L30:
	cmpq	$9999999, -8(%rbp)
	jbe	L31
	movq	$0, -8(%rbp)
	jmp	L33
L34:
	call	_get_time
	fstpt	-128(%rbp)
	movl	$50, %edi
	call	_factorial_while
	movq	%rax, -24(%rbp)
	call	_get_time
	fstpt	-144(%rbp)
	fldt	-144(%rbp)
	fldt	-128(%rbp)
	fsubrp	%st, %st(1)
	fldt	-80(%rbp)
	faddp	%st, %st(1)
	fstpt	-80(%rbp)
	incq	-8(%rbp)
L33:
	cmpq	$9999999, -8(%rbp)
	jbe	L34
	movq	$0, -8(%rbp)
	jmp	L36
L37:
	call	_get_time
	fstpt	-128(%rbp)
	movl	$50, %edi
	call	_factorial_asm
	movq	%rax, -32(%rbp)
	call	_get_time
	fstpt	-144(%rbp)
	fldt	-144(%rbp)
	fldt	-128(%rbp)
	fsubrp	%st, %st(1)
	fldt	-96(%rbp)
	faddp	%st, %st(1)
	fstpt	-96(%rbp)
	incq	-8(%rbp)
L36:
	cmpq	$9999999, -8(%rbp)
	jbe	L37
	movq	$0, -8(%rbp)
	jmp	L39
L40:
	call	_get_time
	fstpt	-128(%rbp)
	movl	$50, %edi
	call	_factorial_for
	movq	%rax, -40(%rbp)
	call	_get_time
	fstpt	-144(%rbp)
	fldt	-144(%rbp)
	fldt	-128(%rbp)
	fsubrp	%st, %st(1)
	fldt	-112(%rbp)
	faddp	%st, %st(1)
	fstpt	-112(%rbp)
	incq	-8(%rbp)
L39:
	cmpq	$9999999, -8(%rbp)
	jbe	L40
	movq	-40(%rbp), %rcx
	movq	-32(%rbp), %rsi
	movq	-24(%rbp), %rdi
	movq	-16(%rbp), %r9
	movq	-112(%rbp), %rax
	movl	-104(%rbp), %edx
	movq	%rax, 48(%rsp)
	movl	%edx, 56(%rsp)
	movq	-96(%rbp), %rax
	movl	-88(%rbp), %edx
	movq	%rax, 32(%rsp)
	movl	%edx, 40(%rsp)
	movq	-80(%rbp), %rax
	movl	-72(%rbp), %edx
	movq	%rax, 16(%rsp)
	movl	%edx, 24(%rsp)
	movq	-64(%rbp), %rax
	movl	-56(%rbp), %edx
	movq	%rax, (%rsp)
	movl	%edx, 8(%rsp)
	movq	%rcx, %r8
	movq	%rsi, %rcx
	movq	%rdi, %rdx
	movq	%r9, %rsi
	leaq	LC2(%rip), %rdi
	movl	$0, %eax
	call	_printf
	movl	$0, %eax
	leave
	ret
LFE12:
	.literal8
	.align 3
LC0:
	.long	2696277389
	.long	1051772663
	.section __TEXT,__eh_frame,coalesced,no_toc+strip_static_syms+live_support
EH_frame1:
	.set L$set$0,LECIE1-LSCIE1
	.long L$set$0
LSCIE1:
	.long	0x0
	.byte	0x1
	.ascii "zR\0"
	.byte	0x1
	.byte	0x78
	.byte	0x10
	.byte	0x1
	.byte	0x10
	.byte	0xc
	.byte	0x7
	.byte	0x8
	.byte	0x90
	.byte	0x1
	.align 3
LECIE1:
.globl _get_time.eh
_get_time.eh:
LSFDE1:
	.set L$set$1,LEFDE1-LASFDE1
	.long L$set$1
LASFDE1:
	.long	LASFDE1-EH_frame1
	.quad	LFB7-.
	.set L$set$2,LFE7-LFB7
	.quad L$set$2
	.byte	0x0
	.byte	0x4
	.set L$set$3,LCFI0-LFB7
	.long L$set$3
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$4,LCFI1-LCFI0
	.long L$set$4
	.byte	0xd
	.byte	0x6
	.align 3
LEFDE1:
.globl _factorial_recursive.eh
_factorial_recursive.eh:
LSFDE3:
	.set L$set$5,LEFDE3-LASFDE3
	.long L$set$5
LASFDE3:
	.long	LASFDE3-EH_frame1
	.quad	LFB8-.
	.set L$set$6,LFE8-LFB8
	.quad L$set$6
	.byte	0x0
	.byte	0x4
	.set L$set$7,LCFI3-LFB8
	.long L$set$7
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$8,LCFI4-LCFI3
	.long L$set$8
	.byte	0xd
	.byte	0x6
	.align 3
LEFDE3:
.globl _factorial_while.eh
_factorial_while.eh:
LSFDE5:
	.set L$set$9,LEFDE5-LASFDE5
	.long L$set$9
LASFDE5:
	.long	LASFDE5-EH_frame1
	.quad	LFB9-.
	.set L$set$10,LFE9-LFB9
	.quad L$set$10
	.byte	0x0
	.byte	0x4
	.set L$set$11,LCFI6-LFB9
	.long L$set$11
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$12,LCFI7-LCFI6
	.long L$set$12
	.byte	0xd
	.byte	0x6
	.align 3
LEFDE5:
.globl _factorial_asm.eh
_factorial_asm.eh:
LSFDE7:
	.set L$set$13,LEFDE7-LASFDE7
	.long L$set$13
LASFDE7:
	.long	LASFDE7-EH_frame1
	.quad	LFB10-.
	.set L$set$14,LFE10-LFB10
	.quad L$set$14
	.byte	0x0
	.byte	0x4
	.set L$set$15,LCFI8-LFB10
	.long L$set$15
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$16,LCFI9-LCFI8
	.long L$set$16
	.byte	0xd
	.byte	0x6
	.align 3
LEFDE7:
.globl _factorial_for.eh
_factorial_for.eh:
LSFDE9:
	.set L$set$17,LEFDE9-LASFDE9
	.long L$set$17
LASFDE9:
	.long	LASFDE9-EH_frame1
	.quad	LFB11-.
	.set L$set$18,LFE11-LFB11
	.quad L$set$18
	.byte	0x0
	.byte	0x4
	.set L$set$19,LCFI10-LFB11
	.long L$set$19
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$20,LCFI11-LCFI10
	.long L$set$20
	.byte	0xd
	.byte	0x6
	.align 3
LEFDE9:
.globl _main.eh
_main.eh:
LSFDE11:
	.set L$set$21,LEFDE11-LASFDE11
	.long L$set$21
LASFDE11:
	.long	LASFDE11-EH_frame1
	.quad	LFB12-.
	.set L$set$22,LFE12-LFB12
	.quad L$set$22
	.byte	0x0
	.byte	0x4
	.set L$set$23,LCFI12-LFB12
	.long L$set$23
	.byte	0xe
	.byte	0x10
	.byte	0x86
	.byte	0x2
	.byte	0x4
	.set L$set$24,LCFI13-LCFI12
	.long L$set$24
	.byte	0xd
	.byte	0x6
	.align 3
LEFDE11:
	.subsections_via_symbols
