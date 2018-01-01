/* ************************************************************************* */
/* Organizacion del Computador II                                            */
/*                                                                           */
/*   Implementacion de la funcion fourCombine                                */
/*                                                                           */
/* ************************************************************************* */

#include "filters.h"
#include <math.h>
#include <stdio.h>
#include <time.h>

void C_fourCombine(uint8_t* src, uint32_t srcw, uint32_t srch,
                   uint8_t* dst, uint32_t dstw, uint32_t dsth __attribute__((unused))) {

	//Igual que siempre casteamos los punteros a las imagenes y recorremos la matriz
	unsigned int (*src_matrix)[srcw] = (unsigned int (*)[srcw]) src;
	unsigned int (*dst_matrix)[dstw] = (unsigned int (*)[dstw]) dst;

	for (unsigned int f = 0; f < srch; f++) {
		for (unsigned int c = 0; c < srcw; c++) {

			RGBA *p_d1 = (RGBA*) &dst_matrix[(int)f/2][(int)c/2];
			RGBA *p_d2 = (RGBA*) &dst_matrix[(int)f/2][(int)c/2 + srcw/2];
			RGBA *p_d3 = (RGBA*) &dst_matrix[(int)f/2 + srch/2][(int)c/2];
			RGBA *p_d4 = (RGBA*) &dst_matrix[(int)f/2 + srch/2][(int)c/2 + srcw/2];

            RGBA *p_s = (RGBA*) &src_matrix[f][c];
            
            if(c % 2 == 0 && f % 2 == 0){
				p_d1->b = p_s->b;
				p_d1->g = p_s->g;
				p_d1->r = p_s->r;
				p_d1->a = p_s->a;
			}
			if(c % 2 == 1 && f % 2 == 0){
				p_d2->b = p_s->b;
				p_d2->g = p_s->g;
				p_d2->r = p_s->r;
				p_d2->a = p_s->a;
			}
			if(c % 2 == 0 && f % 2 == 1){			
				p_d3->b = p_s->b;
				p_d3->g = p_s->g;
				p_d3->r = p_s->r;
				p_d3->a = p_s->a;
			}
			if(c % 2 == 1 && f % 2 == 1){
				p_d4->b = p_s->b;
				p_d4->g = p_s->g;
				p_d4->r = p_s->r;
				p_d4->a = p_s->a;	
			}
			
		}
	}

}