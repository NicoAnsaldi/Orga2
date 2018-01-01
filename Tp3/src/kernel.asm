; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

%include "imprimir.mac"


global start

;; GDT
extern GDT_DESC
extern IDT_DESC
extern inicializar_gdt_tareas
extern idt_inicializar
extern mmu_inicializar_dir_kernel
extern mmu_inicializar_dir_zombi
extern tss_inicializar
extern inicializar_gdt_tareas
extern sched_inicializar

;; VIDEO
extern iniciar_pantalla_mapa
extern print_nombre

extern resetear_pic
extern habilitar_pic  

;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;
iniciando_mr_msg db     'Iniciando kernel (Modo Real)...'
iniciando_mr_len equ    $ - iniciando_mr_msg

iniciando_mp_msg db     'Iniciando kernel (Modo Protegido)...'
iniciando_mp_len equ    $ - iniciando_mp_msg

;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;

;; Punto de entrada del kernel.
BITS 16
start:
    ; Deshabilitar interrupciones
    cli
  

    ; Cambiar modo de video a 80 X 50
    mov ax, 0003h
    int 10h ; set mode 03h
    xor bx, bx
    mov ax, 1112h
    int 10h ; load 8x8 font

    ; Imprimir mensaje de bienvenida
    imprimir_texto_mr iniciando_mr_msg, iniciando_mr_len, 0x07, 0, 3
    

    ; Habilitar A20
    call habilitar_A20
    ; Cargar la GDT
    lgdt [GDT_DESC ]
    ; Setear el bit PE del registro CR0
    mov eax , cr0
    or eax , 0x1
    mov cr0 , eax
    ;xchg bx,bx
    ; Saltar a modo protegido
    jmp  8<<3:modo_protegido
BITS 32
modo_protegido:
    
    ; Establecer selectores de segmentos
    xor eax , eax
    mov ax, 9<<3
    mov ds, ax
    mov es, ax
    mov gs, ax
    mov ss, ax
    mov ax, 12<<3
    mov fs, ax

    

    ; Establecer la base de la pila
    mov ebp , 0x27000
    mov esp , 0x27000

    
    ; Imprimir mensaje de bienvenida
    imprimir_texto_mp iniciando_mp_msg, iniciando_mp_len, 0x07, 0, 0
    ;xchg bx,bx
    
    ; Inicializar pantalla
    call iniciar_pantalla_mapa
    
    ; Inicializar el manejador de memoria
    mov eax, 0x27000
    mov cr3, eax

    ; Inicializar el directorio de paginas
    ; Cargar directorio de paginas
    call mmu_inicializar_dir_kernel


    ; Habilitar paginacion
    mov eax, cr0
    or eax, 0x80000000 ; pon un 1 adelante de todo del cr0
    mov cr0, eax
    call print_nombre
      
    ; Inicializar tss
    ; Inicializar tss de la tarea Idle
    call tss_inicializar
    call inicializar_gdt_tareas


    ; Inicializar la IDT
    ; Cargar IDT
    lidt [IDT_DESC]
    call idt_inicializar
    

    ; Cargar tarea inicial
    mov ax, 13<<3
    ltr ax
    ;xchg bx,bx
    
    ; Configurar controlador de interrupciones
    ; Habilitar interrupciones
    configurando_pic:
    call resetear_pic
    call habilitar_pic  
    
    sti
    ;xchg bx, bx
    

    ; Inicializar el scheduler
    call sched_inicializar
    
    ;int 66

    ; Saltar a la primera tarea: Idle
    jmp 14<<3:0





    ; Ciclar infinitamente (por si algo sale mal...)
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $
    jmp $

;; -------------------------------------------------------------------------- ;;

%include "a20.asm"
