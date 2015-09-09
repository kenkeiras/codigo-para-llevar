global _start

;;;  Definiciones básicas
%define ADDR [static_mem]

section .data
static_mem:     dd 0

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
        cmp edx, 4
        je L3

    cmp edx, 8
        je L2

    cmp edx, 12
        je L1

    cmp edx, 16
        je L0

;;; Default
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
