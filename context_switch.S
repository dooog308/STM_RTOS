.syntax unified

.global PendSV_Handler
.global schedule
.global curTCB
.type PendSV_Handler,%function
.type schedule,%function
.type curTCB,%object

PendSV_Handler:
	.align 4
	MOV R0, #1
	MSR basepri, R0
	MRS R0, PSP
	ISB
	LDR R3, =curTCB
	LDR R2, [R3]
	STMDB R0!, {R4-R11}
	STR R0, [R2]
	STMDB SP!, {R14}
	DSB
	ISB
	BL schedule
	LDMIA sp!, {R14}
	LDR R3, =curTCB
	LDR R2, [R3]
	LDR R0, [R2]
	DSB
	ISB
	LDMIA R0!, {R4-R11}
	MSR PSP, R0
	MOV R0, #0
	MSR basepri, R0
	DSB
	ISB
	BX R14
	NOP
	.align 4

