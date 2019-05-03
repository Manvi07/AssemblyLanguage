%include "io.mac"

.DATA
    Matrix1         db  "-------------------Matrix1-------------------", 0
    Matrix2         db  "-------------------Matrix2-------------------", 0
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

    cmp [R2], word 0
    je abort
    cmp [C2], word 0
    je abort

    mov BX, [C1]
    cmp BX, [R2]
    jne abort

    
    imul  BX, [R1]
    mov [P1], BX
    mov CX, 0
    mov EDX, M1
    PutStr prompt_1
    nwln
    call read_matrix
    
    mov CX, 0
    mov EDX, M2
    mov BX, [C2]
    imul  BX, [R2]
    mov [P2], BX
    PutStr prompt_2
    nwln
    call read_matrix

  mov EAX, M1
    mov BX , [C1]
    mov DX, [P1]
    call print_matrix

    nwln
    mov EAX, M2
    mov BX , [C2]
    mov DX, [P2]
    call print_matrix

    mov EAX, M1
    mov EBX, M2
    mov ECX, M3
    call matrix_mult

; nwln
;     PutCh ' ' 
;     PutInt [M3]

  

    mov BX, [R1]
    imul BX, [C2]
    mov [P2], BX
    nwln
    mov EAX, M3
    mov BX , [C2]
    mov DX, [P2]
    call print_matrix




done:
    .EXIT

abort:
    PutStr Dim
    nwln
    jmp done

read_matrix:
    cmp CX, BX
    je  stop_reading
    GetInt AX
    mov [EDX], AX
    add EDX, 2
    inc CX
    jmp read_matrix

stop_reading:
    nwln
    ret

print_matrix:
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

matrix_mult:
    mov EAX, M1
    
    mov ECX, M3
    mov [i], word 0
start:
    mov EBX, M2
    mov [j], word 0
    cmp [i], word 0
    je mult_val
    mov DX, word [C1]
    imul DX, 2
    movzx EDX, DX
    add EAX, EDX
    ; PutCh 'p'
    ; PutInt [EAX]
    ; nwln
    mult_val:
        mov [sum], word 0
        mov [temp1], word 0
        mov [temp2] , word 0
        cmp [j], word 0
        je mult_row
        add EBX, 2
        
        mult_row:
            movzx EDX, word [i]
            ; imul [i], 
            ; mov EAX, A
            ; add EAX, EDX
            mov DX, [temp1]
            imul DX, 2
            mov [temp], DX
            ; PutInt [temp]
            mov EDX, [temp]
            ; PutInt [EAX + EDX]
            mov EDX, [EAX + EDX]
            mov [k], EDX
            ; nwln
            inc word [temp1]
            
            mult_col:
                mov DX, [temp2]
                imul DX, 2
                imul DX, [C2]
                mov [temp], DX
                mov EDX, [temp]
                ; PutInt [EBX + EDX]
                mov DX, [EBX + EDX]
                ; PutInt DX
                ; PutCh ' '
                ; PutInt [k]
                ; PutCh ' '
                imul DX, [k]
                ; nwln
                ; PutInt DX
                add [sum], DX
                ; nwln
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
                
ret
