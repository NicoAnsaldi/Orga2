/* ************************************************************************* */
/* Organizacion del Computador II                                            */
/*                                                                           */
/*   Implementacion de la funcion Zoom                                       */
/*                                                                           */
/* ************************************************************************* */

#include "filters.h"
#include <math.h>
#include <stdio.h>
#include <time.h>

void C_maxCloser(uint8_t* src, uint32_t srcw, uint32_t srch,
                 uint8_t* dst, uint32_t dstw, uint32_t dsth __attribute__((unused)), float val) {

	unsigned int (*src_matrix)[srcw] = (unsigned int (*)[srcw]) src;
	unsigned int (*dst_matrix)[dstw] = (unsigned int (*)[dstw]) dst;


	
	for (unsigned int f = 0; f < srch; f++) {
		for (unsigned int c = 0; c < srcw; c++) {
			RGBA* p_d = (RGBA*) &dst_matrix[f][c];
            //RGBA* p_s = (RGBA*) &src_matrix[f][c];

			
			if ((f < 4) | (f > srch - 4)){

				p_d->r = 255;
				p_d->g = 255;
				p_d->b = 255;
				p_d->a = 255;

			}

			if ((c < 4) | (c > srcw - 4)){

				p_d->r = 255;
				p_d->g = 255;
				p_d->b = 255;
				p_d->a = 255;

			}

		}
	}




	for (unsigned int f = 3; f < srch-3; f++) {
		for (unsigned int c = 3; c < srcw-3; c++) {
			RGBA* p_d = (RGBA*) &dst_matrix[f][c];
            RGBA* p_s = (RGBA*) &src_matrix[f][c];

			 unsigned int bmax = p_s->b;
			 unsigned int gmax = p_s->g;
			 unsigned int rmax = p_s->r;

			for (unsigned int i = 0; i < 7; i++)
			{
				for (unsigned int j = 0; j < 7; j++)
				{
					if (bmax < ((((RGBA*) &src_matrix[f -3 + i][(c - 3 + j)])->b))  )
					{
						bmax = (((RGBA*) &src_matrix[f -3 + i][c - 3 + j])->b);
					}
					
					if (gmax < (((RGBA*) &src_matrix[f -3 + i][c - 3 + j])->g))
					{
						gmax = (((RGBA*) &src_matrix[f -3 + i][c - 3 + j])->g);
					} 
					if (rmax < (((RGBA*) &src_matrix[f -3 + i][c - 3 + j])->r))
					{
						rmax = (((RGBA*) &src_matrix[f -3 + i][c - 3 + j])->r);
					}
				}
			}
						p_d->r = p_s->r*(1 - val) + rmax*val;
			p_d->g = p_s->g*(1 - val) + gmax*val;
			p_d->b = p_s->b*(1 - val) + bmax*val;
			p_d->a = p_s->a;
			
		}
	}
}

