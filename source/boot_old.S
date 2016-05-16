// To keep this in the first portion of the binary.
.section ".text.boot"

// Make _start global.

.globl _start

.equ    CPSR_MODE_USER,         		0x10
.equ    CPSR_MODE_FIQ,          		0x11
.equ    CPSR_MODE_IRQ,          		0x12
.equ    CPSR_MODE_SVR,          		0x13
.equ    CPSR_MODE_ABORT,        		0x17
.equ    CPSR_MODE_UNDEFINED,    		0x1B
.equ    CPSR_MODE_SYSTEM,       		0x1F

// See ARM section A2.5 (Program status registers)
.equ    CPSR_IRQ_INHIBIT,       		0x80
.equ    CPSR_FIQ_INHIBIT,       		0x40
.equ    CPSR_THUMB,   				0x20

_start:
	ldr pc, _vector_reset
	ldr pc, _vector_undefined
	ldr pc, _vector_swi
	ldr pc, _vector_prefetch_abort
	ldr pc, _vector_data_abort
	ldr pc, _vector_reserved
	ldr pc, _vector_irq
	ldr pc, _vector_fiq

	.align 4

	_vector_reset: .word __vector_reset
	_vector_undefined: .word __vector_undefined
	_vector_swi: .word __vector_swi
	_vector_prefetch_abort: .word __vector_prefetch_abort
	_vector_data_abort: .word __vector_data_abort
	_vector_reserved: .word __vector_reserved
	_vector_irq: .word __vector_irq
	_vector_fiq: .word __vector_fiq

__vector_reset:
	mov 	r0, #0x8000
	mov	r1, #0x0000
	ldmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r1!, {r2, r3, r4, r5, r6, r7, r8, r9}
	ldmia	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia	r1!, {r2, r3, r4, r5, r6, r7, r8, r9}

	// We're going to use interrupt mode, so setup the interrupt mode
	// stack pointer which differs to the application stack pointer:
	mov r0, #(CPSR_MODE_IRQ | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT )
	msr cpsr_c, r0
	mov sp, #0x8000

	mov r0, #(CPSR_MODE_ABORT | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT )
	msr cpsr_c, r0
	mov sp, #0x7000

	mov r0, #(CPSR_MODE_SYSTEM | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT )
	msr cpsr_c, r0
	mov sp, #0x6000

	mov r0, #(CPSR_MODE_UNDEFINED | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT )
	msr cpsr_c, r0
	mov sp, #0x5000

	// Switch back to supervisor mode (our application mode) and
	// set the stack pointer towards the end of RAM. Remember that the
	// stack works its way down memory, our heap will work it's way
	// up memory toward the application stack.
	mov r0, #(CPSR_MODE_SVR | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT )
	msr cpsr_c, r0

	mov     sp, #0x4000
	sub     sp, sp, #0x4

	bl kernel_main

	// halt
halt:
	wfe
	b halt

__vector_undefined:
	sub r14, r14, #4
	stmfd r13!, {r0-r3, r14}

	bl common_undef_handler

	ldmfd r13!, {r0-r3, pc}^

__vector_swi:
	sub r14, r14, #4
	stmfd r13!, {r0-r3, r14}

	bl common_swi_handler

	ldmfd r13!, {r0-r3, pc}^

__vector_prefetch_abort:
	sub r14, r14, #4
	stmfd r13!, {r0-r3, r14}

	bl common_pref_handler

	ldmfd r13!, {r0-r3, pc}^

__vector_data_abort:
	sub r14, r14, #4
	stmfd r13!, {r0-r3, r14}

	bl common_Dabt_handler

	ldmfd r13!, {r0-r3, pc}^

__vector_reserved:
	sub r14, r14, #4
	stmfd r13!, {r0-r3, r14}

	bl common_res_handler

	ldmfd r13!, {r0-r3, pc}^

__vector_irq:
	sub r14, r14, #4
	stmfd r13!, {r0-r3, r14}

	bl common_irq_handler

	ldmfd r13!, {r0-r3, pc}^

__vector_fiq:
	sub r14, r14, #4
	stmfd r13!, {r0-r3, r14}

	bl common_fiq_handler

	ldmfd r13!, {r0-r3, pc}^


.globl _enable_interrupts
_enable_interrupts:
	mrs r0, cpsr
	bic r0, r0, #0x80
	msr cpsr_c, r0

	mov pc, lr

.globl _disable_interrupts
_disable_interrupts:
	mrs r0, cpsr
	orr r0,r0,#0x80
	msr cpsr_c,r0

	mov pc, lr

.globl start_MMU
start_MMU:
	mov r2, #0
	mcr p15,0,r2,c7,c7,0
	mcr p15,0,r2,c8,c7,0
	mcr p15,0,r2,c7,c10,4

	mvn r2,#0
	mcr p15,0,r2,c3,c0,0

	mcr p15,0,r0,c2,c0,0

	mov r2,#0x1
	mcr p15,0,r2,c1,c0,0

	bx lr


.globl PUT32
PUT32:
    str r1,[r0]
    bx lr

.globl PUT16
PUT16:
    strh r1,[r0]
    bx lr

.globl GET32
GET32:
    ldr r0,[r0]
    bx lr

.globl BRANCHTO
BRANCHTO:
    bx r0

.globl dummy
dummy:
    bx lr
