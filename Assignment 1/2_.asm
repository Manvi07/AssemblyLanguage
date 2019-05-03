; Manvi Gupta
; Roll No. B17092
; Assignment 5
; Computer Organization Lab CS201P 
; Solution 2

%include "io.mac"

.DATA
	prompt_1	 	db	"Number of students: ", 0
	prompt_2		db	"Number of subjects: ", 0
    prompt_3        db  "Roll number of Student ", 0
    colon           db  ": ", 0
    prompt_4        db  "Score in s", 0
    count           dw  0
    Max_marks Times 1000 dw 0
    Output_1        db  "Roll No ", 0
    Output_2        db  " got max marks in s", 0
	
.UDATA
    N           resw    1
    M           resw    1
    mark        resw    1
    roll_num    resw    1
    Max_roll    resw    1000

.CODE
    .STARTUP
    mov AX, [count]
    inc AX
    PutStr prompt_1
    GetInt [N]
    PutStr prompt_2
    GetInt [M]
    nwln
    
    Start:
        mov EDX, Max_marks
        mov ECX, Max_roll
        mov [count],word 0
        cmp AX, [N]
        jg result
        PutStr prompt_3
        PutInt AX
        PutStr colon
        GetInt [roll_num]  

    Score_card:
        PutStr prompt_4
        PutInt [count]
        PutStr colon
        GetInt [mark]
        mov BX,[mark]
        cmp [EDX],BX
        jg continue

        assign:
            mov [EDX], BX
            mov BX, [roll_num]
            mov [ECX], BX

    continue:
        add [count], word 1
        add EDX, 2
        add ECX, 2
        mov BX, [M]
        cmp [count], BX
        jl Score_card
        inc AX
        cmp AX, [N]
        jle Start

    mov [count],word 0
    mov BX, [count]
    mov EDX, Max_marks
    mov ECX, Max_roll
    nwln
    result:
        cmp BX, [M]
        je done
        PutStr Output_1
        PutInt [ECX]
        PutStr Output_2
        PutInt BX
        PutStr colon
        PutInt [EDX]
        nwln
        Add ECX , 2
        Add EDX , 2
        Add BX,word 1
        jmp result

done:
	.EXIT
	