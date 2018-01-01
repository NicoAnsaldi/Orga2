/* ************************************************************************* */
/* Organizacion del Computador II                                            */
/*                                                                           */
/*   Implementacion de la funcion convertRGBtoYUV y convertYUVtoRGB          */
/*                                                                           */
/* ************************************************************************* */

#include "filters.h"
#include <math.h>
#include <stdio.h>
#include <time.h>


void C_convertRGBtoYUV(uint8_t* src, uint32_t srcw, uint32_t srch, 
	uint8_t* dst, uint32_t dstw, uint32_t dsth __attribute__((unused))) {

	/* Casteamos el puntero a la imagen destino y fuente como un arreglo
	de punteros a los pixeles de las 2 imagenes */
	unsigned int (*src_matrix)[srcw] = (unsigned int (*)[srcw]) src;
	unsigned int (*dst_matrix)[dstw] = (unsigned int (*)[dstw]) dst;
	
	//Notar que recorremos por pixeles y no por las componentes de los mismos
	for (unsigned int f = 0; f < srch; f++) {
		for (unsigned int c = 0; c < srcw; c++) {
			/* Aca casteamos una vez mas, lo que hacemos es castear el puntero al
			pixel que estamos viendo (tanto en la imagen destino como la fuente)
			a un puntero en las estructuras descriptas en filters.h */
			YUVA *p_d = (YUVA*) &dst_matrix[f][c]; 
            RGBA *p_s = (RGBA*) &src_matrix[f][c];

			//Aca calculamos lo pedido en base al pixel de la imagen fuente           
			int y = (((p_s->r*66    + p_s->g*129   + p_s->b*25    + 128) >> 8) + 16);
			int u = (((p_s->r*(-38) + p_s->g*(-74) + p_s->b*112   + 128) >> 8) + 128);
			int v = (((p_s->r*112   + p_s->g*(-94) + p_s->b*(-18) + 128) >> 8) + 128);

			//Luego lo asignamos al pixel en la imagen destino y lo saturamos despues
			p_d->y = y;
			p_d->u = u;
			p_d->v = v;
			p_d->a = p_s->a;

			if (y > 255){
				p_d->y = 255;
			}
			if (u > 255){
				p_d->u = 255;
			}
			if (v > 255){	
				p_d->v = 255;
			}
			if (y < 0){
				p_d->y = 0;
			}
			if (u < 0){
				p_d->u = 0;
			}
			if (v < 0){	
				p_d->v = 0;
			}
			//NO SATURA Y ESTA HARCODEADO LA LONGITUD DEL CICLO Y DE LA IMAGEN
		}
	}
	//Esto lo hacemos para cada pixel, o sea, recorremos toda la matriz
}

void C_convertYUVtoRGB(uint8_t* src, uint32_t srcw, uint32_t srch,
                       uint8_t* dst, uint32_t dstw, uint32_t dsth __attribute__((unused))) {

	//Muy parecida a la funcion de arriba, solo cambia la conversion
	unsigned int (*src_matrix)[srcw] = (unsigned int (*)[srcw]) src;
	unsigned int (*dst_matrix)[dstw] = (unsigned int (*)[dstw]) dst;

	for (unsigned int f = 0; f < srch; f++) {
		for (unsigned int c = 0; c < srcw; c++) {
			YUVA *p_s = (YUVA*) &src_matrix[f][c];
            RGBA *p_d = (RGBA*) &dst_matrix[f][c];

			int r = ((298*(p_s->y - 16) + 409*(p_s->v - 128) + 128) >> 8);
			int g = ((298*(p_s->y - 16) - 100*(p_s->u - 128) - 208*(p_s->v - 128) + 128) >> 8);
			int b = ((298*(p_s->y - 16) + 516*(p_s->u - 128) + 128) >> 8)	;
			
			p_d->r = r;
			p_d->g = g;
			p_d->b = b;
			p_d->a = p_s->a;


			if (r > 255){
				p_d->r = 255;
			}
			if (g > 255){
				p_d->g = 255;
			}
			if (b > 255){	
				p_d->b = 255;
			}
			if (r < 0){
				p_d->r = 0;
			}
			if (g < 0){
				p_d->g = 0;
			}
			if (b < 0){	
				p_d->b = 0;
			}
		}
	}
}
