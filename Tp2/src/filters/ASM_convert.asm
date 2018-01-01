extern C_convertYUVtoRGB
extern C_convertRGBtoYUV

section .data

mascaraR: db 0xFF,0xFF,0xFF,0x0, 0xFF,0xFF,0xFF,0x0, 0xFF,0xFF,0xFF,0x0, 0xFF,0xFF,0xFF,0x0
mascaraG: db 0xFF,0xFF,0x0,0xFF, 0xFF,0xFF,0x0,0xFF, 0xFF,0xFF,0x0,0xFF, 0xFF,0xFF,0x0,0xFF
mascaraB: db 0xFF,0x0,0xFF,0xFF, 0xFF,0x0,0xFF,0xFF, 0xFF,0x0,0xFF,0xFF, 0xFF,0x0,0xFF,0xFF
mascaraA: db 0x0,0xFF,0xFF,0xFF, 0x0,0xFF,0xFF,0xFF, 0x0,0xFF,0xFF,0xFF, 0x0,0xFF,0xFF,0xFF

mascaraV: db 0xFF,0x0,0xFF,0xFF, 0xFF,0x0,0xFF,0xFF, 0xFF,0x0,0xFF,0xFF, 0xFF,0x0,0xFF,0xFF

mascaraU: db 0xFF,0xFF,0x0,0xFF, 0xFF,0xFF,0x0,0xFF, 0xFF,0xFF,0x0,0xFF, 0xFF,0xFF,0x0,0xFF

mascaraY: db 0xFF,0xFF,0xFF,0x0, 0xFF,0xFF,0xFF,0x0, 0xFF,0xFF,0xFF,0x0, 0xFF,0xFF,0xFF,0x0

mascara66: dd 66,66,66,66
mascara38: dd -38, -38, -38, -38
mascara112: dd 112, 112, 112, 112
mascara129: dd 129, 129, 129, 129
mascara74: dd -74, -74, -74, -74
mascara94: dd -94, -94, -94, -94
mascara25: dd 25, 25, 25, 25
mascara18: dd -18, -18, -18, -18

mascara298:  dd 298, 298, 298, 298
mascara409:  dd 409, 409, 409, 409
mascaram100: dd -100,-100,-100,-100
mascara516:  dd 516, 516, 516, 516
mascaram208: dd -208,-208,-208,-208
mascaram16:  dd -16, -16, -16, -16
mascaram128: dd -128, -128, -128,-128 

mascara128: dd 128 ,128 ,128 ,128 
mascara16: dd 16, 16, 16, 16
 
mascara1: dw 16, 16, 16, 16, 128, 128, 128, 128 
mascara2: dw 128, 128, 128, 128, 00, 00, 00, 00 

mascarashuffle: db 0xc,0x8,0x4,0x0, 0xd,0x9,0x5,0x1, 0xe,0xa,0x6,0x2, 0xf,0xb,0x7,0x3 ; yuva
; xmm4 = |v3|v2|v1|v0| a3|a2|a1|a0| u3|u2|u1|u0| y3|y2|y1|y0
;         0  1  2  3   4  5  6  7   8  9  a  b   c  d  e  f
;tengo eso, quiero esto
; xmm4 = |y3|u3|v3|a3| y2|u2|v2|a2| y1|u1|v1|a1| y0|u0|v0|a0|

mascarashuffle1: db 0x3,0x7,0xb,0xf,0x2,0x6,0xa,0xe,0x1,0x5,0x9,0xd,0x0,0x4,0x8,0xc ; yuva
; xmm4 = |r0|r1|r2|r3| g0|g1|g2|g3| b0|b1|b2|b3| a0|a1|a2|a3|
;         0  1  2  3   4  5  6  7   8  9  a  b   c  d  e  f
;tengo eso, quiero esto
;xmm4 = |r3|g3|b3|a3| r2|g2|b2|a2| r1|g1|b1|a1| r0|g0|b0|a0|

section .text
global ASM_convertYUVtoRGB
global ASM_convertRGBtoYUV




ASM_convertRGBtoYUV:
;void C_convertRGBtoYUV(uint8_t* src, uint32_t srcw, uint32_t srch, 
;uint8_t* dst, uint32_t dstw, uint32_t dsth __attribute__((unused))) {
;rdi = *src
;esi =  srcw = cols
;edx = srch = filas
;rcx = *dst
;r8d = dstw
;r9d = dsth


mov r11, rcx
xor rax , rax	
mov eax , esi ; eax = cols
mul edx 	  ; rax = cols * filas
shr rax , 2 ; rax/4
mov rcx , rax

.ciclo:

	movdqu xmm1, [rdi] 	; xmm1 = | r3 g3 b3 a3 | r2 g2 b2 a2 | r1 g1 b1 a1 | r0 g0 b0 a0
	movdqu xmm2, [rdi] 	; xmm2 = | r3 g3 b3 a3 | r2 g2 b2 a2 | r1 g1 b1 a1 | r0 g0 b0 a0 
	movdqu xmm3, [rdi] 	; xmm3 = | r3 g3 b3 a3 | r2 g2 b2 a2 | r1 g1 b1 a1 | r0 g0 b0 a0
	movdqu xmm13, [rdi] ; xmm13 = | r3 g3 b3 a3 | r2 g2 b2 a2 | r1 g1 b1 a1 | r0 g0 b0 a0  ;esto me lo guardo para despues

	pxor xmm10, xmm10

	movdqu xmm0, [mascaraR]
	pblendvb xmm1, xmm10, xmm0 ; xmm1 = 15 | r3 0 0 0 | r2 0 0 0 | r1 0 0 0 | r0 0 0 0 | 0

	movdqu xmm0, [mascaraG]
	pblendvb xmm2, xmm10, xmm0 ; xmm2 = | 0 g3 0 0 | 0 g2 0 0 | 0 g1 0 0 | 0 g0 0 0

	movdqu xmm0, [mascaraB]
	pblendvb xmm3, xmm10, xmm0 ; xmm3 = | 0 0 b3 0 | 0 0 b2 0 | 0 0 b1 0 | 0 0 b0 0

	movdqu xmm0, [mascaraA]
	pblendvb xmm13, xmm10, xmm0 ; xmm13 = | 0 0 0 a3 | 0 0 0 a2 | 0 0 0 a1 | 0 0 0 a0

	psrld xmm1, 24
	psrld xmm2, 16 	
	psrld xmm3, 8	

	movdqu xmm4, [mascara66]
	pmulld xmm4, xmm1 ; xmm4 = | r3*66 | r2*66 | r1*66 | r0*66

	movdqu xmm5, [mascara38]
	pmulld xmm5, xmm1 ; xmm5 = | r3*-38 | r2*-38 | r1*-38 | r0*-38

	movdqu xmm6, [mascara112]
	pmulld xmm6, xmm1 ; xmm6 = | r3*112 | r2*112 | r1*112 | r0*112

	movdqu xmm7, [mascara129]
	pmulld xmm7, xmm2 ; xmm7 = | g3*129 | g2*129 | g1*129 | g0*129

	movdqu xmm8, [mascara74]
	pmulld xmm8, xmm2 ; xmm8 = | g3*-74 | g2*-74 | g1*-74 | g0*-74

	movdqu xmm9, [mascara94]
	pmulld xmm9, xmm2 ; xmm9 = | g3*-94 | g2*-94 | g1*-94 | g0*-94

	movdqu xmm10, [mascara25]
	pmulld xmm10, xmm3 ; xmm10 = | b3*25 | b2*25 | b1*25 | b0*25

	movdqu xmm11, [mascara112]
	pmulld xmm11, xmm3 ; xmm11 = | b3*112 | b2*112 | b1*112 | b0*112

	movdqu xmm12, [mascara18]
	pmulld xmm12, xmm3 ; xmm12 = | b3*-18 | b2*-18 | b1*-18 | b0*-18

	paddd xmm4, xmm7
	paddd xmm4, xmm10 ; xmm4 = | r3*66 + g3*129  + b3*25 | r2*66 + g2*129 + b2*25 | r1*66 + g1*129  + b1*25 | r0*66 + g0*129 + b0*25

	paddd xmm5, xmm8
	paddd xmm5, xmm11 ; xmm5 = | r3*-38 + g2*-74 + b0*112 | r1*-38 + g1*-74 + b1*112 | r2*-38 + g2*-74 + b2*112 | r3*-38 + g3*-74 + b3*112

	paddd xmm6, xmm9
	paddd xmm6, xmm12 ; xmm6 = | r3*112 + g2*-94 + b0*-18 | r1*112 + g1*-94 + b1*-18 | r2*112 + g2*-94 + b2*-18 | r3*112 + g3*-94 + b3*-18

	movdqu xmm10, [mascara128]
	paddd xmm4, xmm10	;xmm4 = | r0*66 + g0*129  + b0*25 +128 | r1*66 + g1*129 + b1*25 +128 | r2*66 + g2*129  + b2*25 +128 | r3*66 + g3*129 + b3*25 +128
	paddd xmm5, xmm10	;xmm5 = | r0*-38 + g0*-74 + b0*112 +128| r1*-38 + g1*-74 + b1*112 +128| r2*-38 + g2*-74 + b2*112 +128| r3*-38 + g3*-74 + b3*112 +128
	paddd xmm6, xmm10	;xmm6 = | r0*112 + g0*-94 + b0*-18 +128| r1*112 + g1*-94 + b1*-18 +128| r2*112 + g2*-94 + b2*-18 +128| r3*112 + g3*-94 + b3*-18 +128

	psrad xmm4, 8	;xmm4 = | r0*66 + g0*129  + b0*25 +128 >> 8| r1*66 + g1*129 + b1*25 +128 | r2*66 + g2*129  + b2*25 +128 | r3*66 + g3*129 + b3*25 +128
	psrad xmm5, 8 	;xmm5 = | r0*-38 + g0*-74 + b0*112 +128 >> 8| r1*-38 + g1*-74 + b1*112 +128| r2*-38 + g2*-74 + b2*112 +128| r3*-38 + g3*-74 + b3*112 +128
	psrad xmm6, 8	;xmm6 = | r0*112 + g0*-94 + b0*-18 +128 >> 8| r1*112 + g1*-94 + b1*-18 +128| r2*112 + g2*-94 + b2*-18 +128| r3*112 + g3*-94 + b3*-18 +128

	movdqu xmm10, [mascara16]
	paddd xmm4, xmm10	;xmm4 = | r0*66 + g0*129  + b0*25 +16 | r1*66 + g1*129 + b1*25 +16 | r2*66 + g2*129  + b2*25 +16 | r3*66 + g3*129 + b3*25 +16
	movdqu xmm10, [mascara128]
	paddd xmm5, xmm10	;xmm5 = | r0*-38 + g0*-74 + b0*112 +128| r1*-38 + g1*-74 + b1*112 +128| r2*-38 + g2*-74 + b2*112 +128| r3*-38 + g3*-74 + b3*112 +128
	paddd xmm6, xmm10	;xmm6 = | r0*112 + g0*-94 + b0*-18 +128| r1*112 + g1*-94 + b1*-18 +128| r2*112 + g2*-94 + b2*-18 +128| r3*112 + g3*-94 + b3*-18 +128

	; xmm4 = |y3|y2|y1|y0
	; xmm5 = |u3|u2|u1|u0
	; xmm6 = |v3|v2|v1|v0

	; falta pakear con saturacion 3 veces y shufflear

	packssdw xmm4, xmm5 ; xmm4 = |u3|u2|u1|u0|y3|y2|y1|y0

	packssdw xmm6, xmm13 ; xmm6 = |v3|v2|v1|v0|a3|a2|a1|a0

	packuswb xmm4, xmm6  ; xmm4 = |v3|v2|v1|v0|a3|a2|a1|a0|u3|u2|u1|u0|y3|y2|y1|y0

	movdqu xmm10, [mascarashuffle]

	pshufb xmm4, xmm10	; xmm4 =

	movdqu [r11] , xmm4

	add rdi , 16
	add r11 , 16 

dec rcx
jnz .ciclo


ret




ASM_convertYUVtoRGB:
;void C_convertYUVtoRGB(uint8_t* src, uint32_t srcw, uint32_t srch,
;uint8_t* dst, uint32_t dstw, uint32_t dsth __attribute__((unused))) {
;rdi = *src
;esi =  srcw = cols
;edx = srch = filas
;rcx = *dst
;r8d = dstw
;r9d = dsth


mov r11, rcx
xor rax , rax	
mov eax , esi ; eax = cols
mul edx 	  ; rax = cols * filas
shr rax , 2 ; rax/4
mov rcx , rax

.ciclo:

	movdqu xmm1, [rdi] 	; xmm1 = | y3 u3 v3 a3 | y2 u2 v2 a2 | y1 u1 v1 a1 | y0 u0 v0 a0
	movdqu xmm2, [rdi] 	; xmm2 = | y3 u3 v3 a3 | y2 u2 v2 a2 | y1 u1 v1 a1 | y0 u0 v0 a0 
	movdqu xmm3, [rdi] 	; xmm3 = | y3 u3 v3 a3 | y2 u2 v2 a2 | y1 u1 v1 a1 | y0 u0 v0 a0
	movdqu xmm13, [rdi] ; xmm13 =| y3 u3 v3 a3 | y2 u2 v2 a2 | y1 u1 v1 a1 | y0 u0 v0 a0  ;esto me lo guardo para despues

	pxor xmm10, xmm10

	movdqu xmm0, [mascaraA]
	pblendvb xmm13, xmm10, xmm0 ; xmm13 = | 0 0 0 a3 | 0 0 0 a2 | 0 0 0 a1 | 0 0 0 a0

	movdqu xmm0, [mascaraV]
	pblendvb xmm3, xmm10, xmm0 ; xmm3 = | 0 0 v3 0 | 0 0 v2 0 | 0 0 v1 0 | 0 0 v0 0

	movdqu xmm0, [mascaraU]
	pblendvb xmm2, xmm10, xmm0 ; xmm2 = | 0 u3 0 0 | 0 u2 0 0 | 0 u1 0 0 | 0 u0 0 0

	movdqu xmm0, [mascaraY]
	pblendvb xmm1, xmm10, xmm0 ; xmm1 = | y3 0 0 0 | y2 0 0 0 | y1 0 0 0 | y0 0 0 0

	psrld xmm1, 24
	psrld xmm2, 16 	
	psrld xmm3, 8	

	movdqu xmm4, [mascaram16]
	paddd xmm4, xmm1 ; xmm1 = | y0-16 | y1-16 | y2-16 | y3-16

	movdqu xmm10, [mascara298]
	pmulld xmm4, xmm10 ; xmm4 = | y0-16*298  | y1-16*298 | y2-16*298 | y3-16*298
	movdqu xmm7, xmm4 ; copia de xmm4
	movdqu xmm8, xmm4 ; copia de xmm4

	movdqu xmm10, [mascaram128]
	paddd xmm3, xmm10 ; xmm3 = | v0-128 | v1-128 | v2-128 | v3-128
	movdqu xmm5, xmm3 ; copia de xmm3

	movdqu xmm10, [mascaram128]
	paddd xmm2, xmm10 ; xmm3 = | u0-128 | u1-128 | u2-128 | u3-128
	movdqu xmm6, xmm2 ; copia de xmm2


	movdqu xmm10, [mascaram100]
	pmulld xmm2, xmm10 ; xmm2 = | u0-128*-100 | u1-128*-100 | u2-128*-100 | u3-128*-100

	movdqu xmm10, [mascara516]
	pmulld xmm6, xmm10 ; xmm6 = | u0-128*516 | u1-128*516 | u2-128*516 | u3-128*516

	movdqu xmm10, [mascara409]
	pmulld xmm3, xmm10 ; xmm3 = | v0-128*409 | v1-128*409 | v2-128*409 | v3-128*409

	movdqu xmm10, [mascaram208]
	pmulld xmm5, xmm10 ; xmm5 = | v0-128*-208 | v1-128*-208 | v2-128*-208 | v3-128*-208



	paddd xmm4, xmm3 ; xmm4 = | y0-16*298 + v0-128*409  | y1-16*298 + v1-128*409 | y2-16*298 + v2-128*409 | y3-16*298 + v3-128*409

	paddd xmm7, xmm2 ; xmm7 = | y0-16*298 + u0-128*-100 | y1-16*298 + u1-128*-100 | y2-16*298 + u2-128*-100 | y3-16*298 + u3-128*-100
	paddd xmm7, xmm5 ; xmm7 = | y0-16*298 + u0-128*-100 + v0-128*-208| y1-16*298 + u1-128*-100 + v1-128*-208| y2-16*298 + u2-128*-100 + v2-128*-208| y3-16*298 + u3-128*-100 + v3-128*-208

	paddd xmm8, xmm6 ; xmm8 = | y0-16*298 + u0-128*516 | y1-16*298 + u1-128*516| y2-16*298 + u2-128*516| y3-16*298 + u3-128*516

	movdqu xmm10, [mascara128]
	paddd xmm4, xmm10
	paddd xmm7, xmm10 ; les sumo a los 3 128
	paddd xmm8, xmm10

	psrad xmm4, 8	
	psrad xmm7, 8 ; shifteo los 3 por 8 
	psrad xmm8, 8	


	;hasta aca tengo el resultado de las 3 operaciones en 4 pixeles sin saturar

	; falta pakear con saturacion 3 veces y shufflear

	packusdw xmm4, xmm7 ; xmm4 = |r0|r1|r2|r3|g0|g1|g2|g3

	packusdw xmm8, xmm13 ; xmm8 = b0|b1|b2|b3|a0|a1|a2|a3|

	packuswb xmm4, xmm8 ; xmm4 = |r0|r1|r2|r3|g0|g1|g2|g3|b0|b1|b2|b3|a0|a1|a2|a3|

	movdqu xmm10, [mascarashuffle]

	pshufb xmm4, xmm10	;xmm4 = |r3|g3|b3|a3|r2|g2|b2|a2|r1|g1|b1|a1|r0|g0|b0|a0|

	movdqu [r11] , xmm4

	add rdi , 16
	add r11 , 16 

dec rcx
jnz .ciclo


ret