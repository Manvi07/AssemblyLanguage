; Manvi Gupta
; Roll No. B17092
; Assignment 5
; Computer Organization Lab CS201P 
; Solution 1 

%include "io.mac"

.DATA
	prompt_msg_1 	db 	"Enter your string: ", 0
	output_msg   	db 	"Encoded string: ", 0
	prompt_continue db  "Would you like to terminate? (y/n) ",0

.UDATA
	input_str	resb	1000
	c 			resb	1

.CODE
	.STARTUP
encode:
	PutStr  prompt_msg_1
	GetStr	input_str, 100				;length of string
	mov EBX, input_str
	PutStr output_msg
	repeat:
		mov AL, [EBX]
		cmp AL, 0
		je check
		cmp AL, '9'
		jg not_in_range
		cmp AL, '0'
		jl not_in_range
		cmp AL, '3'
		jl add_40 
		cmp AL, '9'
		jl add_14
		je make_0
	add_40:
		add AL, 40
		jmp not_in_range
	add_14:
		add AL, 14
		jmp not_in_range
	make_0:
		sub AL, 9

	not_in_range:
		PutCh AL
		inc EBX
		jmp repeat

	check:
		nwln
		PutStr prompt_continue
		GetCh [c]
		mov AH ,[c]
		cmp AH, 'Y'
		je done
		cmp AH, 'y'
		je done
		cmp AH, 'n'
		je encode
		jne check

  nwln
done:
 	.EXIT