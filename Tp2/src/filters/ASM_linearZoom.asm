;Hipotesis malloc guarda la memoria alineada
section .data
mascaraS: db 0x0,0x1,0x2,0x3,0xff,0xff,0xff,0xff,0x4,0x5,0x6,0x7,0xff,0xff,0xff,0xff
mascaraSb: db 0xff,0xff,0xff,0xff,0x4,0x5,0x6,0x7,0xff,0xff,0xff,0xff,0x0,0x1,0x2,0x3

section .text
global ASM_linearZoom
extern C_linearZoom

ASM_linearZoom:
;rdi: src
;esi: srcw
;edx: srch
;rcx: dst
;r8d, dstw
;r9d, dsth
push rbp
mov rbp, rsp
push r12
push r13
push r14
push rbx
push r15
sub rsp, 8


shl rdx, 32
shr rdx, 32
shl rsi, 32
shr rsi, 32
shl r8, 32
shr r8, 32
shl r9, 32
shr r9, 32
xor rax, rax ;Limpio rax para hacer la multiplicacion
mov eax, esi ;Paso las columnas de la imagen source
mov r10, rdx ;r10 = filas
dec r10
mul r10 ;Rax = (filas-1)*columnas
mov r11, rax ;r9 = (filas-1)*columnas
shr r11, 2 ;Divido por 4, r11 = ((filas-1)*columnas)/4

;Punteros para la imagen source
xor r10, r10 ;Contador para saber cuando llegue al final de una fila

mov r12, rdi ;r12 es mi segundo puntero para moverme por la imagen
mov rax, 4
mul rsi ;Para moverse entre pixeles
add r12, rax

;Punteros para la imagen destino
mov r13, rcx ;r13 segundo puntero para recorrer la imagen destino
mov r14, rcx ;Lo uso para no perder la direccion de la primera fila
mov rax, 4
mul r8 ;Para moverse entre pixeles 
add r13, rax
add r14, rax ;Ahora r14 apunta a la tercera fila
add r14, rax

.ciclo:
	cmp r11, 0
	je .PrimeraUltimaFila

	movdqa xmm15, [rdi] ; xmm0 = |p3|p2|p1|p0|
	movaps xmm14, [r12] ;xmm1 = |y3|y2|y1|y0|

	pxor xmm7, xmm7 ; xmm7 = |0|0|0|0| Lo uso para desempaquetar, me aprovecho de que los pixeles son unsigned
	
	movdqa xmm2, xmm15 ;xmm2 = |p3|p2|p1|p0|
	movdqa xmm6, xmm15 ;xmm6 = |p3|p2|p1|p0|
	punpcklbw xmm6, xmm7 ;xmm6 = |p1|p0|
	punpckhbw xmm2, xmm7 ;xmm2 = |p3|p2|

	movdqa xmm8, xmm6 ;xmm8 = |p1|p0|
	psrldq xmm8, 8 ;xmm8 = |0|p1|
	paddw xmm8, xmm6 ;xmm8 = |p1|p1+p0|
	psrlw xmm8, 1 ;xmm8 = |p1>>1|p1+p0>>1|
	pslldq xmm8, 8 ;xmm8 = |p1+p0>>1|0|

	movdqa xmm9, xmm2 ;xmm9 = |p3|p2|
	psrldq xmm9, 8 ;xmm9 = |0|p3|
	paddw xmm9, xmm2 ;xmm9 = |p3|p3+p2|
	psrlw xmm9, 1 ;xmm9 = |p3>>1|p3+p2>>1|
	pslldq xmm9, 8 ;xmm9 = |p3+p2>>1|0|

	movdqa xmm10, xmm2 ;xmm10 = |p3|p2|
	pslldq xmm10, 8 ;xmm10 = |p2|0|
	paddw xmm10, xmm6 ;xmm10 = |p2+p1|p0|
	psrlw xmm10, 1 ;xmm10 = |p2+p1>>1|p0>>1|
	psrldq xmm10, 8 ;xmm10 = |0|p2+p1>>1|

	movdqa xmm4, xmm14 ;xmm4 = |y3|y2|y1|y0|
	movdqa xmm1, xmm14 ;xmm1 = |y3|y2|y1|y0|	
	punpcklbw xmm4 ,xmm7 ;xmm4 = |y1|y0|
	punpckhbw xmm1, xmm7 ;xmm1 = |y3|y2|

	paddw xmm6, xmm4 ;xmm6 = |p1+y1|p0+y0|
	psrlw xmm6, 1 ;xmm6 = |p1+y1>>1|p0+y0>>1|
	paddw xmm2, xmm1 ;xmm2 = |p3+y3|p2+y2|
	psrlw xmm2, 1 ;xmm6 = |p3+y3>>1|p2+y2>>1|

	movdqa xmm11, xmm6 ;xmm11 = |p1+y1>>1|p0+y0>>1|
	psrldq xmm11, 8 ;xmm11 = |0|p1+y1>>1|
	paddw xmm11, xmm6 ;xmm11 = |p1+y1>>1|(p1+y1>>1)+(p0+y0>>1)|
	psrlw xmm11, 1 ;xmm11 = |p1+y1>>1|((p1+y1>>1)+(p0+y0>>1))>>1|
	pslldq xmm11, 8 ;xmm11 = |0|((p1+y1>>1)+(p0+y0>>1))>>1|

	movdqa xmm12, xmm2 ;xmm12 = |p3+y3>>1|p2+y2>>1|
	psrldq xmm12, 8 ;xmm12 = |0|p3+y3>>1|
	paddw xmm12, xmm2 ;xmm12 = |p3+y3>>1|(p3+y3>>1)+(p2+y2>>1)|
	psrlw xmm12, 1 ;xmm12 = |p3+y3>>1|((p3+y3>>1)+(p2+y2>>1))>>1|
	pslldq xmm12, 8 ;xmm12 = |0|((p3+y3>>1)+(p2+y2>>1))>>1|	

	movdqa xmm13, xmm2 ;xmm13 = |p3+y3>>1|p2+y2>>1|
	pslldq xmm13, 8 ;xmm13 = |p2+y2>>1|0|
	paddw xmm13, xmm6 ;xmm13 = |(p2+y2>>1)+(p1+y1>>1)|p0+y0>>1|
	psrlw xmm13, 1 ;xmm13 = |((p2+y2>>1)+(p1+y1>>1))>>1|p0+y0>>1|
	psrldq xmm13, 8 ;xmm13 = |0|((p2+y2>>1)+(p1+y1>>1))>>1|

	;Packeo lo que tengo
	packuswb xmm6, xmm2 ;xmm6 = |p3+y3>>1|p2+y2>>1|p1+y1>>1|p0+y0>>1|
	packuswb xmm8, xmm9 ;xmm8 = |p3+p2>>1|0|p1+p0>>1|0|
	packuswb xmm10, xmm10 ;xmm10 = |0|p2+p1>>1|0|p2+p1>>1|
	packuswb xmm11, xmm12 ;xmm11 = |((p3+y3>>1)+(p2+y2>>1))>>1|0|((p1+y1>>1)+(p0+y0>>1))>>1|0|
	packuswb xmm13, xmm13 ;xmm13 = |0|((p2+y2>>1)+(p1+y1>>1))>>1|0|((p2+y2>>1)+(p1+y1>>1))>>1|
	
	;Me construyo los pixeles para pasarlos de una a la imagen destino
	;filas pares
	movaps xmm0, xmm15 ;xmm0 = |p3|p2|p1|p0| Primeros 4 pixeles 
	movaps xmm1, xmm15 ;xmm1 = |p3|p2|p1|p0| los otros 3  pixeles que faltan
	;Primeros 4 pixeles
	movups xmm4, [mascaraS]
	pshufb xmm0, xmm4 ;xmm0 = |0|p1|0|p0|
	movdqa xmm14, xmm8 ;xmm14 = |p3+p2>>1|0|p1+p0>>1|0|
	pslldq xmm14, 4 ;xmm14 = |0|p1+p0>>1|0|0|
	psrldq xmm14, 4 ;xmm14 = |0|0|p1+p0>>1|0|
	por xmm14, xmm10 ;xmm14 = |0|p2+p1>>1|p1+p0>>1|p2+p1>>1|
	movups xmm5, [mascaraSb]
	pshufb xmm14, xmm5 ;xmm14 = |0|p2+p1>>1|0|p1+p0>>1|
	por xmm0, xmm14 ;xmm0 |p2+p1>>1|p1|p1+p0>>1|p0|
	;Los otros 3 
	psrldq xmm1, 8 ;xmm1 = |0|0|p3|p2|
	pshufb xmm1, xmm4 ;xmm1 = |0|p3|0|p2|
	psrldq xmm8, 12 ;xmm8 = |0|0|0|p3+p2>>1| por ahora no me lo guardo
	pslldq xmm8, 4 ;xmm8 = |0|0|p3+p2>>1|0|
	por xmm1, xmm8 ;xmm1 = |0|p3|p3+p2>>1|p2|

	;filas impares
	;Primeros 4 pixeles
	movaps xmm3, xmm6 ;xmm3 = |p3+y3>>1|p2+y2>>1|p1+y1>>1|p0+y0>>1|
	pshufb xmm3, xmm4 ;xmm3 = |0|p1+y1>>1|0|p0+y0>>1|
	movaps xmm7, xmm11 ;xmm7 = |((p3+y3>>1)+(p2+y2>>1))>>1|0|((p1+y1>>1)+(p0+y0>>1))>>1|0|
	pslldq xmm7, 4 ;xmm7 = |0|((p1+y1>>1)+(p0+y0>>1))>>1|0|0|
	psrldq xmm7, 4 ;xmm7 = |0|0|((p1+y1>>1)+(p0+y0>>1))>>1|0|
	por xmm3, xmm11 ;xmm3 = |fruta|p1+y1>>1|((p1+y1>>1)+(p0+y0>>1))>>1|p0+y0>>1|
	pslldq xmm3, 4 ;xmm3 = |p1+y1>>1|((p1+y1>>1)+(p0+y0>>1))>>1|p0+y0>>1|0|
	psrldq xmm3, 4 ;xmm3 = |0|p1+y1>>1|((p1+y1>>1)+(p0+y0>>1))>>1|p0+y0>>1|
	pslldq xmm13, 12 ;xmm13 = |((p2+y2>>1)+(p1+y1>>1))>>1|0|0|0|
	por xmm3, xmm13 ;xmm3 = |((p2+y2>>1)+(p1+y1>>1))>>1|p1+y1>>1|((p1+y1>>1)+(p0+y0>>1))>>1|p0+y0>>1|
	;Los otros 3
	psrldq xmm6, 8 ;xmm6 = |0|0|p3+y3>>1|p2+y2>>1|
	pshufb xmm6, xmm4 ;xmm6 = |0|p3+y3>>1|0|p2+y2>>1|
	psrldq xmm11, 12 ;xmm8 = |0|0|0|((p3+y3>>1)+(p2+y2>>1))>>1| por ahora no me lo guardo
	pslldq xmm11, 4 ;xmm8 = |0|0|((p3+y3>>1)+(p2+y2>>1))>>1|0|
	por xmm6, xmm11 ; xmm6 = |0|p3+y3>>1|((p3+y3>>1)+(p2+y2>>1))>>1|p2+y2>>1|

	;Ahora lo paso a la imagen destino viendo de llenar la última columna 
	;filas impares
	movaps [r13], xmm0
	add r13, 16
	movaps [r13], xmm1
	add r13, 12 ;Si bien aca tengo un pixel mas, me paro justo en el último pixel porque en el próximo ciclo lo piso de nuevo
	;filas pares
	movaps [r14], xmm3
	add r14, 16
	movaps [r14], xmm6
	add r14, 12 ;Aca pasa lo mismo que arriba, la razón es para dejar bien el ciclo para la próxima pasada

	add r10, 4
	cmp r10d, esi
	je .llegueAlFinal
	;Si no llegue al final de la fila relleno, en la imagen destino, ese pixel molesto para volver a empezar el ciclo
	add rdi, 12
	add r12, 12
	movups xmm0, [rdi] ;xmm0 = |p6|p5|p4|p3| Solo necesito los pixeles p3 y p4
	movups xmm1, [r12] ;xmm1 = |y6|y5|y4|y3| 
	movaps xmm2, xmm0 ;xmm2 = |p6|p5|p4|p3|
	pxor xmm3, xmm3 ;Limpio xmm3 para hacer el umpack
	punpcklbw xmm0, xmm3 ;Xmm0 = |p4|p3| Solo me importa la parte baja la alta no la voy a usar 
	punpcklbw xmm1, xmm3 ;xmm2 = |y4|y3|

	movaps xmm4, xmm0 ;xmm4 = |p4|p3|
	movaps xmm5, xmm0 ;xmm5 = |p4|p3|
	psrldq xmm5, 8 ;xmm5 = |0|p4|
	paddw xmm4, xmm5 ;xmm4 = |p4|p4+p3|
	psrlw xmm4, 1 ;xmm4 = |p4>>1|p4+p3>>1|

	paddw xmm1, xmm0 ;xmm2 = |p4+y4|p3+y3|
	psrlw xmm1, 1 ;xmm2 = |p4+y4>>1|p3+y3>>1|
	movaps xmm6, xmm1 ;xmm6 = |p4+y4>>1|p3+y3>>1|
	psrldq xmm6, 8 ;xmm6 = |0|p4+y4>>1|
	paddw xmm1, xmm6 ;xmm6 = |p4+y4>>1|(p4+y4>>1)+(p3+y3>>1)|
	psrlw xmm1, 1 ;xmm2 = |p4+y4>>1|((p4+y4>>1)+(p3+y3>>1))>>1|

	;Ahora packeo para despues pasarlos a la imagen
	packuswb xmm4, xmm4 ;xmm4 = |p4>>1|p4+p3>>1||p4>>1|p4+p3>>1|
	packuswb xmm1 ,xmm1 ;xmm2 = |p4+y4>>1|((p4+y4>>1)+(p3+y3>>1))>>1||p4+y4>>1|((p4+y4>>1)+(p3+y3>>1))>>1|

	movd [r13], xmm4 
	movd [r14], xmm1

	add r13, 4
	add r14, 4
	add rdi, 4
	add r12, 4
	jmp .sigo
	;Si llegue al final lo que hago es copiar el pixel anterior
	.llegueAlFinal:
		xor r10, r10 ;Limpio r10 para empezar una nueva fila
		sub r13, 4 ;lo hago para completar el último pixel en la imagen destino
		sub r14, 4
		;Con 32 bits me alcanza para levantar un pixel, por eso lo hago en un registro de 64 bits 
		mov ebx, dword [r13] 
 		mov r15d, dword [r14] 
 		;Ahora que tengo los pixeles avanzo 
 		add r13, 4 
 		add r14, 4
		mov dword [r13], ebx
 		mov dword [r14], r15d 
 		;Ahora recoloco los punteros en la imagen destino
 		add r13, 4 ;Me coloco en la siguiente fila
 		add r14, 4
 		mov rax, 4 ;Lo que les tengo que sumar a mis punteros
 		mul r8 
 		add r13, rax
 		add r14, rax
 		add rdi, 16
 		add r12, 16
	.sigo:
		dec r11
		jmp .ciclo

.PrimeraUltimaFila:
	;Ultima fila
	;Notar que rdi apunta a la ultima fila de la imagen source y r13 apunta a la ultima fila de la imagen destino
	xor r10, r10 ;Contador de columnas
	mov r14, rsi 
	.ciclo2:
		cmp r14, 0
		je .PrimeraFila
		movaps xmm0, [rdi] ;xmm0 = |p3|p2|p1|p0|
		movaps xmm1, xmm0 ;xmm1 = |p3|p2|p1|p0|
		movaps xmm2, xmm0 ;xmm2 = |p3|p2|p1|p0|
		pxor xmm3, xmm3 ;Lo uso para umpackear

		punpcklbw xmm1, xmm3 ;xmm1 = |p1|p0|
		punpckhbw xmm2, xmm3 ;xmm2 = |p3|p2|

		movaps xmm4, xmm1 ;xmm4 = |p1|p0|
		psrldq xmm4, 8 ;xmm4 = |0|p1|
		paddw xmm4, xmm1 ;xmm4 = |p1|p1+p0|
		psrlw xmm4, 1 ;xmm4 = |p1>>1|p1+p0>>1|
		pslldq xmm4, 8 ;xmm4 = |p1+p0>>1|0|

		movaps xmm5, xmm2 ;xmm5 = |p3|p2|
		pslldq xmm5, 8 ;xmm5 = |p2|0|
		paddw xmm5, xmm1 ;xmm5 = |p2+p1|p0|
		psrlw xmm5, 1 ;xmm5 = |p2+p1>>1|p0>>1|
		psrldq xmm5, 8 ;xmm5 = |0|p2+p1>>1|

		movaps xmm6, xmm2 ;xmm6 = |p3|p2|
		psrldq xmm6, 8 ;xmm6 = |0|p3|
		paddw xmm6, xmm2 ;xmm6 = |p3|p2+p3|
		psrlw xmm6, 1 ;xmm6 = |p3>>1|p2+p3>>1|
		pslldq xmm6, 8 ;xmm6 = |p2+p3>>1|0|

		;Ahora packeo
		packuswb xmm4, xmm4 ;xmm4 = |p1+p0>>1|0|p1+p0>>1|0|
		packuswb xmm5, xmm5 ;xmm5 = |0|p2+p1>>1|0|p2+p1>>1|
		packuswb xmm6, xmm6 ;xmm6 = |p2+p3>>1|0|p2+p3>>1|0|

		;Armo un registro para pasarlo a la imagen destino
		;Los primeros 4 pixeles
		movaps xmm7, xmm0 ;xmm7 = |p3|p2|p1|p0|
		movups xmm8, [mascaraS]
		pshufb xmm7, xmm8 ;xmm7 = |0|p1|0|p0|
		por xmm4, xmm5 ;xmm4 = |p1+p0>>1|p2+p1>>1|p1+p0>>1|p2+p1>>1|
		movups xmm9, [mascaraSb]
		pshufb xmm4, xmm9 ;xmm4 = |0|p2+p1>>1|0|p1+p0>>1|
		por xmm7, xmm4 ;xmm7 |p2+p1>>1|p1|p1+p0>>1|p0|
		
		;Los otros 3 pixeles que puedo construir
		psrldq xmm0, 8 ;xmm0 = |0|0|p3|p2|
		pshufb xmm0, xmm8 ;xmm0 = |0|p3|0|p2|
		pslldq xmm6, 4 ;xmm6 = |0|p3+p2>>1|0|0| por ahora no me lo guardo
		psrldq xmm6, 4 ;xmm6 = |0|0|p3+p2>>1|0|
		por xmm0, xmm6 ;xmm1 = |0|p3|p3+p2>>1|p2|

		;Paso a la imagen destino
		movaps [r13], xmm7
		add r13, 16
		movaps [r13], xmm0 
		add r13, 12

		add r10, 4
		cmp r10, rsi
		je .completoUltimoPixel
		;Ahora tengo que completar el pixel que me falta para poder ciclar
		add rdi, 12
		movups xmm0, [rdi] ;xmm0 = |p6|p5|p4|p3|
		
		pxor xmm2, xmm2
		punpcklbw xmm0, xmm2 ;xmm0 = |p4|p3| Solo necesito estos dos pixeles

		movaps xmm1, xmm0 ;xmm1 = |p4|p3|
		psrldq xmm1, 8 ;xmm1 = |0|p4|
		paddw xmm1, xmm0 ;xmm1 = |p4|p4+p3|
		psrlw xmm1, 1 ;xmm1 = |p4>>1|p4+p3>>1|

		;Packeo y lo muevo a la imagen destino
		packuswb xmm1, xmm1 ;xmm1 = |p4>>1|p4+p3>>1|p4>>1|p4+p3>>1|
		movd [r13], xmm1
		;Muevo los punteros y vuelvo a ciclar
		add r13, 4
		add rdi, 4
		sub r14, 4
		jmp .ciclo2
		.completoUltimoPixel:
			;Me falta llenar el ultimo pixel de la imagen
			sub r13, 4
			mov r14d, dword [r13]
			add r13, 4
			mov dword [r13], r14d
		

	.PrimeraFila:
	

	mov r14, rcx ;Lo uso para copiar la fila
	mov rax, 4
	mul r8 ;rax = cantidad columnas * 4
	add r14, rax ;Ahora r14 esta en la segunda fila
	xor r15, r15 ;Contador
	
	.ciclo3:
		cmp r15, r8
		je .fin
		mov r10d, dword [r14] ;Cada pixel ocupa 32 bits con un registro de 64 bits me alcanza
		mov dword [rcx], r10d
		add rcx, 4 ;Avanzo los punteros
		add r14, 4
		inc r15
		jmp .ciclo3

.fin:
add rsp, 8
pop r15
pop rbx
pop r14
pop r13
pop r12
pop rbp
ret