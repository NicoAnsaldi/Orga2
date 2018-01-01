
extern C_fourCombine


section .data

primeros: db 0x0,0x1,0x2,0x3,0x8,0x9,0xa,0xb,0x0,0x1,0x2,0x3,0x8,0x9,0xa,0xb
segundos: db 0x4,0x5,0x6,0x7,0xc,0xd,0xe,0xf,0x4,0x5,0x6,0x7,0xc,0xd,0xe,0xf

mascara1: db 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff
;0 el primero, 1 el segundo

section .text
global ASM_fourCombine

ASM_fourCombine:
;void C_fourCombine(uint8_t* src, uint32_t srcw, uint32_t srch,
;uint8_t* dst, uint32_t dstw, uint32_t dsth __attribute__((unused))) {
;rdi = *src
;esi =  srcw = cols
;edx = srch = filas
;rcx = *dst
;r8d = dstw
;r9d = dsth

push rbp
mov rbp, rsp
push r15
push r14
push r13
push r12
push r11
push r10
push r9
push r8

mov r14d, edx; me copio tam de filas
mov r13d, esi; me copio tam de columnas

xor r8, r8
mov r8d, r13d ; r8 = cols
mov rax, 2
mul r8  ;(cols/2)*4 por los 4B de cada pixel
mov r8d, eax ; r8 = (cols/2)*4

xor r12, r12
mov eax, r14d ; rax = filas
mul r13d ; filas*cols
mov r12d, eax 
mov rax, 2 ; ((filas/2)*tam de 1 col)*4
mul r12
mov r12d, eax; r12 = ((filas/2)*tam de 1 col)*4

xor r11, r11
mov r11d, r12d
sub r11d, r8d; r11 = ((filas/2)*tam de 1 col)*4 - (cols/2)*4


xor r10, r10
xor r9, r9
mov r10d, r13d
shr r10 , 3 ; cols/8 porque proceso 8 pixeles; ojo, con imagenes no multiplo de 8
mov r9d, r14d ; r9 es el que se encarga de saber en que fila estoy

movdqu xmm0, [mascara1]
movdqu xmm1, [primeros] 
movdqu xmm2, [segundos]

.ciclof:

	.ciclocpares:
		mov r15d, r10d
		.innerp:

		movdqu xmm3, [rdi]		; xmm3 = p0 | p1 | p2 | p3 |
		movdqu xmm5, xmm3 		; xmm3 = p0 | p1 | p2 | p3 |
		movdqu xmm4, [rdi+16]	; xmm4 = p4 | p5 | p6 | p7 |
		movdqu xmm6, xmm4 		; xmm6 = p4 | p5 | p6 | p7 |

		pshufb xmm3, xmm1 ; xmm3 = p0 | p2 | p0 | p2
		pshufb xmm5, xmm2 ; xmm5 = p1 | p3 | p1 | p3

		pshufb xmm4, xmm1 ; xmm4 = p4 | p6 | p4 | p6
		pshufb xmm6, xmm2 ; xmm6 = p5 | p7 | p5 | p7

		pblendvb xmm3, xmm4, xmm0 ; xmm3 = p0 | p2 | p4 | p6
		pblendvb xmm5, xmm6, xmm0 ; xmm5 = p1 | p3 | p5 | p7

		movdqu [rcx], xmm3
		movdqu [rcx + r8], xmm5


		add rdi, 32
		add rcx, 16
		dec r15
		jnz .innerp
		dec r9

		mov r14, 7
		and r14d, esi
		cmp r14d, 0
		je .ciclocimpares

		sub rdi, 16
		sub rcx, 8

		movdqu xmm3, [rdi]		; xmm3 = p0 | p1 | p2 | p3 |
		movdqu xmm5, xmm3 		; xmm3 = p0 | p1 | p2 | p3 |
		movdqu xmm4, [rdi+16]	; xmm4 = p4 | p5 | p6 | p7 |
		movdqu xmm6, xmm4 		; xmm6 = p4 | p5 | p6 | p7 |

		pshufb xmm3, xmm1 ; xmm3 = p0 | p2 | p0 | p2
		pshufb xmm5, xmm2 ; xmm5 = p1 | p3 | p1 | p3

		pshufb xmm4, xmm1 ; xmm4 = p4 | p6 | p4 | p6
		pshufb xmm6, xmm2 ; xmm6 = p5 | p7 | p5 | p7

		pblendvb xmm3, xmm4, xmm0 ; xmm3 = p0 | p2 | p4 | p6
		pblendvb xmm5, xmm6, xmm0 ; xmm5 = p1 | p3 | p5 | p7

		movdqu [rcx], xmm3
		movdqu [rcx + r8], xmm5

		add rdi, 32
		add rcx, 16



	.ciclocimpares:
		
		mov r15d, r10d

		.innerimp:
		movdqu xmm3, [rdi] ; xmm3 = p8 | p9 | p10 | p11
		movdqu xmm5, xmm3 ; xmm3 = p8 | p9 | p10 | p11
		movdqu xmm4, [rdi+16] ; xmm4 = p12 | p13 | p14 | p15
		movdqu xmm6, xmm4 ; xmm6 = p12 | p13 | p14 | p15

		pshufb xmm3, xmm1 ; xmm3 = p8 | p10 | p8 | p10
		pshufb xmm5, xmm2 ; xmm5 = p9 | p11 | p9 | p11

		pshufb xmm4, xmm1 ; xmm4 = p12 | p14 | p12 | p14
		pshufb xmm6, xmm2 ; xmm6 = p13 | p15 | p13 | p15

		pblendvb xmm3, xmm4, xmm0 ; xmm3 = p8 | p10 | p12 | p14
		pblendvb xmm5, xmm6, xmm0 ; xmm5 = p9 | p11 | p13 | p15

		movdqu [rcx + r11], xmm3
		movdqu [rcx + r12], xmm5

		add rdi, 32
		add rcx, 16
		dec r15
		jnz .innerimp

		mov r14, 7
		and r14d, esi
		cmp r14d, 0
		je .aca

		sub rdi, 16
		sub rcx, 8
		
		movdqu xmm3, [rdi] ; xmm3 = p8 | p9 | p10 | p11
		movdqu xmm5, xmm3 ; xmm3 = p8 | p9 | p10 | p11
		movdqu xmm4, [rdi+16] ; xmm4 = p12 | p13 | p14 | p15
		movdqu xmm6, xmm4 ; xmm6 = p12 | p13 | p14 | p15

		pshufb xmm3, xmm1 ; xmm3 = p8 | p10 | p8 | p10
		pshufb xmm5, xmm2 ; xmm5 = p9 | p11 | p9 | p11

		pshufb xmm4, xmm1 ; xmm4 = p12 | p14 | p12 | p14
		pshufb xmm6, xmm2 ; xmm6 = p13 | p15 | p13 | p15

		pblendvb xmm3, xmm4, xmm0 ; xmm3 = p8 | p10 | p12 | p14
		pblendvb xmm5, xmm6, xmm0 ; xmm5 = p9 | p11 | p13 | p15

		movdqu [rcx + r11], xmm3
		movdqu [rcx + r12], xmm5

		add rdi, 32
		add rcx, 16
.aca
		dec r9
		jnz .ciclof


.fin:
pop r8
pop r9
pop r10
pop r11
pop r12
pop r13
pop r14
pop r15
pop rbp
ret