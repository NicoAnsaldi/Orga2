	global ASM_maxCloser
extern C_maxCloser
section .rodata

mascaradeval: db 0x0,0x1,0x2,0x3,0x0,0x1,0x2,0x3,0x0,0x1,0x2,0x3,0x0,0x1,0x2,0x3
mascara1: dd 1.0,1.0,1.0,1.0


mascarablanco: db 255,255,255,255, 255,255,255,255, 255,255,255,255, 255,255,255,255
mascarablancoizq: db 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0x0,0x0,0x0,0x0
mascarablancoder: db 0x0,0x0,0x0,0x0, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF, 0xFF,0xFF,0xFF,0xFF
;  0 dest, 1 src

section .text
ASM_maxCloser:
;void C_maxCloser(uint8_t* src, uint32_t srcw, uint32_t srch,
;uint8_t* dst, uint32_t dstw, uint32_t dsth, float val __attribute__((unused))) {
;rdi = *src
;esi =  srcw = cols
;edx = srch = filas
;rcx = *dst
;r8d = dstw
;r9d = dsth
;xmm0 = val
push rbp
mov rbp, rsp
push r8
push r9
push r10
push r11
push r12
push r13
push r14
push r15


xor r13, r13
mov r15, rcx	;dest copia
mov r14, rcx	;dest copia

xor r12, r12
mov r12d, edx	 ;filas
sub r12, 6		;filas a procesar -6

mov r10d, esi
sub r10, 4   ;cols a procesar -4, porque 4? porque necesito que haga los ultimos 4, por mas que se pase de 2 pixeles
shr r10, 2	; cantidad de pxeles que voy a procesar por ciclo -> r9 = (cols-4)/4
;dec r10
mov r9d, r10d

mov r8d, esi
shl r8, 2 ;cols*4

xor r11, r11
add rcx, 12
add rcx, r8
add rcx, r8
add rcx, r8



.ciclof:
	mov r10d, r9d
	.cicloc:
	cmp r10, 1
	je .distintociclo

	movdqu xmm1, [rdi]
	mov r11, rdi
	mov rax, 7

	.ciclito:

	movdqu xmm3, [r11] ; 0 1 2 3
	pmaxub xmm1, xmm3

	add r11, 4
	movdqu xmm3, [r11]
	pmaxub xmm1, xmm3 ; 1 2 3 4

	add r11, 4
	movdqu xmm3, [r11]
	pmaxub xmm1, xmm3 ; 2 3 4 5

	add r11, 4
	movdqu xmm3, [r11]
	pmaxub xmm1, xmm3 ; 3 4 5 6

	cmp rax, 4
	je .copia
	jmp .segui

	.copia:
	movdqu xmm2, xmm3

	.segui
	add r11, 4
	movdqu xmm3, [r11]
	pmaxub xmm1, xmm3 ; 4 5 6 7

	add r11, 4
	movdqu xmm3, [r11]
	pmaxub xmm1, xmm3 ; 5 6 7 8

	add r11, 4
	movdqu xmm3, [r11]
	pmaxub xmm1, xmm3 ; 6 7 8 9


	add r11, r8
	sub r11, 24 ; 6 p x 4B c/u 
	dec rax
	jnz .ciclito
	jmp .combinacion

.distintociclo
	
	movdqu xmm1, [rdi]
	mov r11, rdi
	mov rax, 7
	
	
	.ciclito2:

	movdqu xmm3, [r11]
	pmaxub xmm1, xmm3

	add r11, 4
	movdqu xmm3, [r11]
	pmaxub xmm1, xmm3 ; 1 2 3 4

	add r11, 4
	movdqu xmm3, [r11]
	pmaxub xmm1, xmm3 ; 2 3 4 5

	add r11, 4
	movdqu xmm3, [r11]
	pmaxub xmm1, xmm3 ; 3 4 5 6

	cmp rax, 4
	je .copia2
	jmp .segui2

	.copia2:
	movdqu xmm2, xmm3

	.segui2

	add r11, 4
	movdqu xmm3, [r11]
	pmaxub xmm1, xmm3 ; 4 5 6 7

	psrldq xmm3, 4
	pmaxub xmm1, xmm3 ; 5 6 7 -

	psrldq xmm3, 4
	pmaxub xmm1, xmm3 ; 6 7 - -


	add r11, r8
	sub r11, 16 ; 4 p x 4B c/u 
	dec rax
	jnz .ciclito2
	add rdi, 16

	;listo, ya tengo los 4 maximos por los cuatro pixeles
	;ahora la parte de la combinacion lineal
	.combinacion
	movdqu xmm14, [mascaradeval]  	
	pshufb xmm0 , xmm14 ; xmm0 = |val|val|val|val|

	;pshufd xmm0 , xmm0, 0

	movups xmm15, [mascara1]  	
	subps xmm15 , xmm0	; xmm15 = |1-val|1-val|1-val|1-val|

	; acordemosnos que en xmm2 me guardé el source
	;movdqu xmm2, [rcx]
	movdqu xmm3, xmm2
	movdqu xmm4, xmm2

	pxor xmm2, xmm2

	PUNPCKHBW xmm3, xmm2
	PUNPCKLBW xmm4, xmm2

	movdqu xmm5, xmm3
	movdqu xmm6, xmm3
	movdqu xmm7, xmm4
	movdqu xmm8, xmm4

	PUNPCKHWD xmm5, xmm2 ;p0 cada componente en dwords
	PUNPCKLWD xmm6, xmm2 ;p1
	PUNPCKHWD xmm7, xmm2 ;p2
	PUNPCKLWD xmm8, xmm2 ;p3 

	cvtdq2ps xmm5, xmm5
	cvtdq2ps xmm6, xmm6
	cvtdq2ps xmm7, xmm7
	cvtdq2ps xmm8, xmm8

	mulps xmm5, xmm15 ;p0*(1-val) cada componente
	mulps xmm6, xmm15 ;p1
	mulps xmm7, xmm15 ;p2
	mulps xmm8, xmm15 ;p3


	;ahora con el resultado

	movdqu xmm9, xmm1
	movdqu xmm10, xmm1

	pxor xmm2, xmm2

	PUNPCKHBW xmm9, xmm2
	PUNPCKLBW xmm10, xmm2

	movdqu xmm11, xmm9
	movdqu xmm12, xmm9
	movdqu xmm13, xmm10
	movdqu xmm14, xmm10			

	PUNPCKHWD xmm11, xmm2 ;p0 cada componente en dwords
	PUNPCKLWD xmm12, xmm2 ;p1
	PUNPCKHWD xmm13, xmm2 ;p2
	PUNPCKLWD xmm14, xmm2 ;p3 

	cvtdq2ps xmm11, xmm11
	cvtdq2ps xmm12, xmm12
	cvtdq2ps xmm13, xmm13
	cvtdq2ps xmm14, xmm14

	mulps xmm11, xmm0 ;p0*(val) cada componente
	mulps xmm12, xmm0 ;p1
	mulps xmm13, xmm0 ;p2
	mulps xmm14, xmm0 ;p3

	addps xmm5, xmm11
	addps xmm6, xmm12
	addps xmm7, xmm13
	addps xmm8, xmm14

	cvtps2dq xmm5, xmm5
	cvtps2dq xmm6, xmm6
	cvtps2dq xmm7, xmm7
	cvtps2dq xmm8, xmm8

	packusdw xmm6, xmm5
	packusdw xmm8, xmm7

	packuswb xmm8, xmm6

	movdqu [rcx] , xmm8

	add rdi, 16
	add rcx, 16

	dec r10
	cmp r10d, 0
	jne .cicloc


dec r12
cmp r12d, 0
je .pintadeblanco

;add rdi, 16 ;dejo bien posicionados los punteros para la proxima fila
add rcx, 16

jmp .ciclof




.pintadeblanco:
;rdi = *src
;esi =  srcw = cols
;edx = srch = filas
;rcx = *dst
;r8d = dstw
;r9d = dsth
;xmm0 = val
;r15 dest
;r14 dest
mov r8d, edx ;filas
mov r13d, edx
sub r13, 6 ; filas-6

mov r9d, esi ; cols
shr r9, 2	;cols/4
mov r11d, r9d

mov eax, r13d
mul esi
mov r10, 4
mul r10
mov r10d, eax ; filas-6*cols*4
mov r12, 3


		.ciclofilas:
		movdqu xmm1, [mascarablanco]
		movdqu [r14], xmm1

		add r14, 16
		dec r9
		cmp r9, 0
		jne .ciclofilas

		mov r9d, r11d
		dec r12
		cmp r12d, 0
		jne .ciclofilas

		add r14, r10
		mov r12, 3

		.ciclof2:
		movdqu xmm1, [mascarablanco]
		movdqu [r14], xmm1

		add r14, 16
		dec r9
		cmp r9d, 0
		jne .ciclof2

		mov r9d, r11d
		dec r12
		cmp r12d, 0
		jne .ciclof2

		mov r11d, esi
		mov rax, 4
		mul r11
		mov r11d, eax ;cols*4

		mov r14, r8 ; reutilizo r14, porque los registros son escazos vistee

		.ciclocols: ;faltas las columnas
		movdqu xmm0, [mascarablancoizq]
		movdqu xmm1, [mascarablanco]
		movdqu xmm3, [r15]
		pblendvb xmm3, xmm1, xmm0; o blend
		movdqu [r15], xmm3

		add r15, r11
		dec r8
		cmp r8d, 0
		jne .ciclocols

		sub r15, 16

		.cicloc2:
		movdqu xmm0, [mascarablancoder]
		movdqu xmm1, [mascarablanco]
		movdqu xmm3, [r15]
		pblendvb xmm3, xmm1, xmm0; o blend
		movdqu [r15], xmm3

		sub r15, r11
		dec r14
		cmp r14, 0
		jne .cicloc2




.salir:

pop r15
pop r14
pop r13
pop r12
pop r11
pop r10
pop r9
pop r8
pop rbp
ret