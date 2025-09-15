.global SVCall_Handler
.global SVCall_CHandler

.type SVCall_Handler,%function
.type SVCall_CHandler,%function

SVCall_Handler:
	MRS R0,PSP
	B SVCall_CHandler
