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
	.word	task_global			//加载任务状态内存基地址
	ldr		r1, [pc, #-12]
	ldr		r0, [r1]

	str		r1, [r0, #(0 * 4)]	//r0
	str		r1, [r0, #(1 * 4)]	//r1

	str		r2, [r0, #(2 * 4)]	//r2
	str		r3, [r0, #(3 * 4)]
	str		r4, [r0, #(4 * 4)]
	str		r5, [r0, #(5 * 4)]
	str		r6, [r0, #(6 * 4)]
	str		r7, [r0, #(7 * 4)]
	str		r8, [r0, #(8 * 4)]
	str		r9, [r0, #(9 * 4)]
	str		r10, [r0, #(10 * 4)]
	str		r11, [r0, #(11 * 4)]
	str		r12, [r0, #(12 * 4)]		//r12
	str		r15, [r0, #(15 * 4)]		//r15

	mrs		r1, cpsr
	str		r1, [r0, #(16 * 4)]			//spsr

	mrs		r1, cpsr					//从irq切换到svc
	bic 	r1, r1, #0x1F				// 不可以 bic 	r1, r1, #0xFF,  不知道原因
	orr		r1, r1, # 0x13
	msr		cpsr, r1

	str		r13,[r0, #(13 * 4)]		//13
	str		r14,[r0, #(14 * 4)]		//14

	bl    common_irq_handler		//系统时钟
	bl		task_schedule					//任务调度

//==========================================
	.word	task_global			//加载任务状态内存基地址
	ldr		r1, [pc, #-12]

	ldr		r0, [r1]

  ldr		r2, [r0, #(2 * 4)]			//r2
	ldr		r3, [r0, #(3 * 4)]
	ldr		r4, [r0, #(4 * 4)]
	ldr		r5, [r0, #(5 * 4)]
	ldr		r6, [r0, #(6 * 4)]
	ldr		r7, [r0, #(7 * 4)]
	ldr		r8, [r0, #(8 * 4)]
	ldr		r9, [r0, #(9 * 4)]
	ldr		r10, [r0, #(10 * 4)]
	ldr		r11, [r0, #(11 * 4)]
	ldr		r12, [r0, #(12 * 4)]
	ldr		r13, [r0, #(13 * 4)]
	ldr		r14, [r0, #(14 * 4)]		//r14

	ldr		r1,[r0, #(15 * 4)]			//将pc的值临时放在sp-4中,因为寄存器都恢复后只有sp指向的内存是安全的
	str		r1, [sp, #-4]

	ldr		r1, [r0, #(16 * 4)]			//spsr
	str		r1, [sp, #-8]
	msr		cpsr, r1					//这里也就打开中断了

	ldr		r1, [r0, #(1 * 4)]			//r1
	ldr		r0, [r0, #(0 * 4)]			//r2			//之前将r0 先回复,r1后恢复.将代码放到ads中才发现这个错误

	ldr		pc,[sp, #-4]				//最后恢复pc,跳转

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
