.global TEST
.global PRINTFI
.type TEST, %function
.type PRINTFI, %function
TEST:
	MOV R0, #10
	b PRINTFI
