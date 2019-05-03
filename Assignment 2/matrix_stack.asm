; Manvi Gupta
; Roll No. B17092
; Assignment 6
; Computer Organization Lab CS201P 

%include "io.mac"

.DATA
    Matrix1         db  "-------------------Matrix A-------------------", 0
    Matrix2         db  "-------------------Matrix B-------------------", 0
    Matrix3         db  "------------Output Matrix C-------------------", 0
    prompt1         db  "No of Rows (N): ", 0
    prompt2         db  "No of Columns (M): ", 0
    prompt_1	 	db	"Enter matrix A: ", 0
	prompt_2		db	"Enter matrix B: ", 0
    Dim             db  "Incompatible dimensions. Abort!", 0
    sum             dw    0
    counter         dw    0
    temp1           dw    0
    temp2           dw    0

.UDATA
    M1              resw    100
    M2              resw    100
    M3              resw    100
    R1              resw    1
    C1              resw    1
    R2              resw    1
    C2              resw    1
    P1              resw    1
    P2              resw    1
    count           resw    1
    A               resw    100
    B               resw    100
    C               resw    100
    i               resw    1
    j               resw    1
    k               resw    1
    temp            resd    1

.CODE
    .STARTUP

    PutStr Matrix1
    nwln 
    PutStr prompt1
    GetInt [R1]
    PutStr prompt2
    GetInt [C1]

;CHECK DIMENSIONS
    cmp [R1], word 0
    je abort
    cmp [C1], word 0
    je abort

    PutStr Matrix2
    nwln 
    PutStr prompt1
    GetInt [R2]
    PutStr prompt2
    GetInt [C2]

;CHECK DIMENSIONS
    cmp [R2], word 0
    je abort
    cmp [C2], word 0
    je abort

    mov BX, [C1]
    cmp BX, [R2]
    jne abort


    imul  BX, [R1]
    mov [P1], BX

;READ MATRIX A  
    push  word [P1]
    push M1
    PutStr prompt_1
    nwln
    call read_matrix
    
;READ MATRIX B
    mov BX, [C2]
    imul  BX, [R2]
    mov [P2], BX
    push word [P2]
    push M2
    PutStr prompt_2
    nwln
    call read_matrix

;MULTIPLY MATRICES
    push M1
    push M2
    push M3
    call matrix_mult

;PRINT MATRIX A  
    push word [C1]
    push word [P1]
    push M1
    PutStr Matrix1
    nwln
    call print_matrix  
    nwln

;PRINT MATRIX B
    push word [C2]
    push word [P2]
    push M2
    PutStr Matrix2
    nwln
    call print_matrix
    nwln


;PRINT MATRIX C
    mov BX, [R1]
    imul BX, [C2]
    mov [P2], BX    
    push word [C2]
    push word [P2]
    push M3
    PutStr Matrix3
    nwln
    call print_matrix
    nwln

;EXIT
done:
    .EXIT

abort:
    PutStr Dim
    nwln
    jmp done

;-------------------------------------------------------------------------------------------
;---------------------------------PROCEDURES------------------------------------------------

read_matrix:
    mov CX, 0
    enter 0,0                       ;push EBP       ;mov EBP, ESP
    mov EDX, [EBP+8]
    mov BX, [EBP+12]

    read:
        cmp CX, BX
        je  stop_reading
        GetInt AX
        mov [EDX], AX
        add EDX, 2
        inc CX
        jmp read

stop_reading:
    nwln
    leave                           ;mov ESP, EBP   ;pop EBP
    ret 6

print_matrix:
    enter 0,0                       ;push EBP       ;mov EBP, ESP
    mov EAX, [EBP+8]
    mov DX, [EBP+12]
    mov BX, [EBP+14]
    mov CX, word 0
    mov [count], word 0

    L1:
        PutInt [EAX]
        PutCh ' '
        add EAX, 2
        add [count], word 1
        inc CX 
        cmp [count], BX
        jne L1

        print_nwln:
            cmp CX, DX
            je stop_reading 
            nwln 
            mov [count], word 0
            jmp L1

    stop_print:
        nwln
        leave
        ret 8

matrix_mult:
    enter 0,0
    mov ECX, [EBP+8]
    mov EBX, [EBP+12]
    mov EAX, [EBP+16]
    mov [i], word 0

    start:
        mov EBX, [EBP+12]
        mov [j], word 0
        
        cmp [i], word 0
        je mult_val

        mov DX, word [C1]
        imul DX, 2
        movzx EDX, DX
        add EAX, EDX

        mult_val:
            mov [sum], word 0
            mov [temp1], word 0
            mov [temp2] , word 0
            cmp [j], word 0
            je mult_row
            add EBX, 2
            
            mult_row:
                movzx EDX, word [i]
                mov DX, [temp1]
                imul DX, 2
                mov [temp], DX
                mov EDX, [temp]
                mov EDX, [EAX + EDX]
                mov [k], EDX
                inc word [temp1]
                
                mult_col:
                    mov DX, [temp2]
                    imul DX, 2
                    imul DX, [C2]
                    mov [temp], DX
                    mov EDX, [temp]
                    mov DX, [EBX + EDX]
                    imul DX, [k]
                    add [sum], DX
                    inc word [temp2]
                    mov DH, byte [C1]
                    cmp [temp1], DH
                    jl mult_row
                    mov DX, [sum] 

                    mov [ECX], DX
                    add ECX, 2

                inc word [j]
                mov DH, byte [C2]
                cmp [j], DH
                jl mult_val
            
            inc word [i]
            mov DH, byte [R1]
            cmp [i], DH
            jl start
leave 
ret 12
