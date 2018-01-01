; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

%include "imprimir.mac"


BITS 32

;;i386.h
extern tlbflush
    
extern print
;;Screen.C
extern print_int
extern iniciar_pantalla
extern mostrar_pantalla
extern iniciar_mapa

;; PIC
extern fin_intr_pic1
extern print_isr
;;scheduler
extern sched_proximo_indice

;; var global
extern pantalla_activa
extern cambiar_tipoA
extern cambiar_tipoB
extern cambiar_posicionA
extern cambiar_posicionB
extern print_teclado

extern game_lanzar_zombi
extern game_jugador_mover
extern cambiar_jugador

extern llamar_tlbflush
extern print_reloj

extern modoDebug
extern modoDebugFueActivado
extern print_tss
extern dame_esp
extern mostrar_mapa
extern estoy_en_estado

extern cr0_e
extern cr2_e
extern cr4_e

extern stack1
extern stack2
extern stack3
extern stack4
extern stack5

;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;
;tareas_vivas: dd 0x0000000 

%macro ISR 1
global _isr%1

_isr%1:
    pushad
    ;xchg bx, bx

    ;;cmp byte [modoDebugFueActivado], 1 
    ;;jne .fin


    mov eax, cr0
    mov [cr0_e], eax
    mov eax, cr2
    mov [cr2_e], eax
    mov eax, cr4
    mov [cr4_e], eax

    ;call dame_esp

    mov ebx, eax
    mov eax, [ebx]
    mov [stack1], eax
    mov eax, [ebx+4]
    mov [stack2], eax
    mov eax, [ebx+8]
    mov [stack3], eax
    mov eax, [ebx+12]
    mov [stack4], eax
    mov eax, [ebx+16]
    mov [stack5], ebx
    

    call print_tss

.fin:
jmp 14<<3:0


popad
iret

%endmacro

;;
;; Datos
;; -------------------------------------------------------------------------- ;;
; Scheduler
isrnumero:           dd 0x00000000
isrClock:            db '|/-\'

;;
;; Rutina de atención de las EXCEPCIONES
;; -------------------------------------------------------------------------- ;;
ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR 8
ISR 9
ISR 10
ISR 11
ISR 12
ISR 13
ISR 14
ISR 15
ISR 16
ISR 17
ISR 18
ISR 19


;;
;; Rutina de atención del RELOJ
;; -------------------------------------------------------------------------- ;;

sched_tarea_offset:     dd 0x00
sched_tarea_selector:   dw 0x00


global _isr32
_isr32:
    pushad
    call fin_intr_pic1

    cmp byte [modoDebug], 1
    je .fin

    call proximo_reloj

    call cambiar_jugador
    call llamar_tlbflush
    call sched_proximo_indice

    str cx
    cmp ax, cx
    je .fin

    
    cmp ax , 20
    je .fin
    
   
    ;xchg bx, bx
    mov [sched_tarea_selector], ax
    jmp far [sched_tarea_offset]

    .fin:
    popad
    iret
;;
;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;

texto: db 0,0

global _isr33
_isr33:
pushad
call fin_intr_pic1
xor al, al
in al , 0x60
;xchg bx, bx
.printNum:
push 0x0F ; white
push 0x00 ; fila0
push 0x4E ; columna 78

mov [texto], al ; esto printea la letra arriba a la derecha

mov ebx, eax

push texto ; lo que hay en eax
;xchg bx, bx
call print_teclado
add esp, 16 ; para alinear la pila

mov eax, ebx
;xchg bx, bx

cmp al, 0x2a ; left shift = 2a + 80
je .letraLS

cmp al, 0x36 ; right shift = 36 + 80
je .letraRS

cmp al, 0x15 ; y = 15 + 80
je .letraY

cmp al, 0x11 ; w = 11 + 80
je .letraW

cmp al, 0x1f ; s = 1f + 80
je .letraS

cmp al, 0x1e ; a = 1e + 80
je .letraA

cmp al, 0x20 ; d = 20 + 80
je .letraD
;; jugador 2
cmp al, 0x17 ; i = 17 + 80
je .letraI

cmp al, 0x25 ; k = 25 + 80
je .letraK

cmp al, 0x26 ; l = 26 + 80
je .letraL

cmp al, 0x24 ; j = 24 + 80
je .letraJ

jmp .fin



.letraY:
;xchg bx, bx

cmp byte [modoDebug], 0
je .segui

mov byte [modoDebug], 0
call mostrar_mapa
jmp .fin

.segui:
mov byte [modoDebugFueActivado], 1
call estoy_en_estado
jmp .fin


.letraW:
; si es 1, voy hacia arriba, osea que es una w
push -1
call cambiar_posicionA
add esp, 4
jmp .fin

.letraS:
; si es 0, voy hacia arriba, osea que es una s
push 1
call cambiar_posicionA
add esp, 4
jmp .fin

.letraA:
;xchg bx, bx
; si es 0, voy hacia la izquierda, osea que es una a
push -1
call cambiar_tipoA
add esp, 4
jmp .fin

.letraD:
; si es 1, voy hacia la derecha, osea que es una d
push 1
call cambiar_tipoA
add esp, 4
jmp .fin

.letraI:
; si es 1, voy hacia arriba, osea que es una i
push -1
call cambiar_posicionB
add esp, 4
jmp .fin

.letraK:
; si es 0, voy hacia arriba, osea que es una k
push 1
call cambiar_posicionB
add esp, 4
jmp .fin

.letraL:
; si es 1, voy hacia la derecha, osea que es una L
push 1
call cambiar_tipoB
add esp, 4
jmp .fin

.letraJ:
; si es 0, voy hacia la izquierda, osea que es una j
push -1
call cambiar_tipoB
add esp, 4
jmp .fin

.letraLS:
push 0
call game_lanzar_zombi
add esp, 4
jmp .fin

.letraRS:
push 1
call game_lanzar_zombi
add esp, 4

.fin:
popad
iret

;;
;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;

%define IZQ 0xAAA
%define DER 0x441
%define ADE 0x83D
%define ATR 0x732


global _isr102
_isr102:

pushad
;xchg bx, bx
push eax
call game_jugador_mover
add esp, 4

mov ax, 14<<3
mov [sched_tarea_selector], ax
jmp far [sched_tarea_offset]


.fin:
popad
iret


;; Funciones Auxiliares
;; -------------------------------------------------------------------------- ;;
proximo_reloj:
        pushad
        inc DWORD [isrnumero]
        mov ebx, [isrnumero]
        cmp ebx, 0x4
        jl .ok
                mov DWORD [isrnumero], 0x0
                mov ebx, 0
        .ok:
                add ebx, isrClock
                imprimir_texto_mp ebx, 1, 0x0f, 49, 79

                call print_reloj

        .fin:
        popad
        ret
        
        
