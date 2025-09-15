.syntax unified

.global PendSV_Handler
.global schedule
.global curTCB
.global fir
.type PendSV_Handler,%function
.type schedule,%function
.type curTCB,%object
.type fir,%object

PendSV_Handler:

	CLREX
	MRS R0, PSP
	ISB
	LDR R3, =curTCB
	LDR R2, [R3]
	STMDB R0!, {R4-R11,R14}
	STR R0, [R2]

	MOV R0, #1
	MSR basepri, R0
	DSB
	ISB

	BL schedule

	MOV R0, #0
	MSR basepri, R0

	LDR R3, =curTCB
	LDR R1, [R3]
	LDR R0, [R1]
	DSB
	ISB

	LDMIA R0!, {R4-R11,R14}

	MSR PSP, R0
	DSB
	ISB
	BX R14
	NOP
	.align 4

