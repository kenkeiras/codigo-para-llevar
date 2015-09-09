global _start

;;;  Definiciones básicas
%define ADDR [static_mem]

section .data
static_mem:          dd 0

;;; Tabla de saltos
jump_table:
    dd LDEFAULT ;  0
    dd LDEFAULT ;  1
    dd LDEFAULT ;  2
    dd LDEFAULT ;  3
    dd L3       ;  4
    dd LDEFAULT ;  5
    dd LDEFAULT ;  6
    dd LDEFAULT ;  7
    dd L2       ;  8
    dd LDEFAULT ;  9
    dd LDEFAULT ; 10
    dd LDEFAULT ; 11
    dd L1       ; 12
    dd LDEFAULT ; 13
    dd LDEFAULT ; 14
    dd LDEFAULT ; 15
    dd L0       ; 16


section .text
;;; Código real
_start:
    ;; Probamos los ciclos 4 <-> 16
    mov edx, 4
    times 5000000 call tested_switch

    ;; Probamos los ciclos 8 <-> 12
    mov edx, 8
    times 5000000 call tested_switch

;;; Cerramos correctamente
_end:
        ;; Exit with 0
        mov eax, 1
        xor ebx, ebx
        int 0x80
;;; Código a testear
tested_switch:
    ;; Multiplicamos edx por 4 por que este es el tamaño de las entradas de la tabla
    ;;  ... aunque no estoy demasiado seguro :P
    shl edx, 2

    mov ebx, [jump_table + edx]
    jmp rbx

;;; Default
LDEFAULT:
        mov eax, ADDR      ; #<FDEFINITION object for PRINC>
        mov ecx, 4
        ret

;;; 16
L0:
        mov edx, 4
        mov eax, ADDR      ; #<FDEFINITION object for PRINC>
        mov ecx, 4

    ret

;;; 12
L1:
        mov edx, 8
        mov eax, ADDR      ; #<FDEFINITION object for PRINC>
        mov ecx, 4

    ret

;;; 8
L2:
        mov edx, 12
        mov eax, ADDR      ; #<FDEFINITION object for PRINC>
        mov ecx, 4

    ret

;;; 4
L3:
        mov edx, 16
        mov eax, ADDR      ; #<FDEFINITION object for PRINC>
        mov ecx, 4

    ret
