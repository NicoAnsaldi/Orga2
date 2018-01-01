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

void C_linearZoom(uint8_t* src, uint32_t srcw, uint32_t srch,
                  uint8_t* dst, uint32_t dstw, uint32_t dsth __attribute__((unused))) {

	unsigned int (*src_matrix)[srcw] = (unsigned int (*)[srcw]) src;
	unsigned int (*dst_matrix)[dstw] = (unsigned int (*)[dstw]) dst;

	for (unsigned int f = 0; f < srch-1; f++) {
		for (unsigned int c = 0; c < srcw-1; c++) {

			RGBA* p_d0 = (RGBA*) &dst_matrix[f*2+1][c*2];
			RGBA* p_d1 = (RGBA*) &dst_matrix[f*2+1][c*2+2];
			RGBA* p_d2 = (RGBA*) &dst_matrix[f*2+2+1][c*2];
			RGBA* p_d3 = (RGBA*) &dst_matrix[f*2+2+1][c*2+2];

			RGBA* p_dp1 = (RGBA*) &dst_matrix[f*2+1][c*2+1];
			RGBA* p_dp2 = (RGBA*) &dst_matrix[f*2+1+1][c*2];	
			RGBA* p_dp3 = (RGBA*) &dst_matrix[f*2+1+1][c*2+1];
			RGBA* p_dp4 = (RGBA*) &dst_matrix[f*2+1+1][c*2+2];
			RGBA* p_dp5 = (RGBA*) &dst_matrix[f*2+2+1][c*2+1];

            RGBA* p_s0 = (RGBA*) &src_matrix[f][c];
            RGBA* p_s1 = (RGBA*) &src_matrix[f][c+1];
            RGBA* p_s2 = (RGBA*) &src_matrix[f+1][c];
            RGBA* p_s3 = (RGBA*) &src_matrix[f+1][c+1];

			*p_d0 = *p_s0;
			*p_d1 = *p_s1;
			*p_d2 = *p_s2;
			*p_d3 = *p_s3;

			p_dp1->a = (uint8_t) (((uint16_t) p_s0->a + (uint16_t) p_s1->a)/2);
			p_dp1->b = (uint8_t) (((uint16_t) p_s0->b + (uint16_t) p_s1->b)/2);
			p_dp1->g = (uint8_t) (((uint16_t) p_s0->g + (uint16_t) p_s1->g)/2);
			p_dp1->r = (uint8_t) (((uint16_t) p_s0->r + (uint16_t) p_s1->r)/2);

			p_dp2->a = (uint8_t) (((uint16_t) p_s0->a + (uint16_t) p_s2->a)/2);
			p_dp2->b = (uint8_t) (((uint16_t) p_s0->b + (uint16_t) p_s2->b)/2);
			p_dp2->g = (uint8_t) (((uint16_t) p_s0->g + (uint16_t) p_s2->g)/2);
			p_dp2->r = (uint8_t) (((uint16_t) p_s0->r + (uint16_t) p_s2->r)/2);

			p_dp3->a = (uint8_t) (((uint16_t) p_s0->a + (uint16_t) p_s1->a + (uint16_t) p_s2->a + (uint16_t) p_s3->a)/4);
			p_dp3->b = (uint8_t) (((uint16_t) p_s0->b + (uint16_t) p_s1->b + (uint16_t) p_s2->b + (uint16_t) p_s3->b)/4);
			p_dp3->g = (uint8_t) (((uint16_t) p_s0->g + (uint16_t) p_s1->g + (uint16_t) p_s2->g + (uint16_t) p_s3->g)/4);
			p_dp3->r = (uint8_t) (((uint16_t) p_s0->r + (uint16_t) p_s1->r + (uint16_t) p_s2->r + (uint16_t) p_s3->r)/4);

			p_dp4->a = (uint8_t) (((uint16_t) p_s1->a + (uint16_t) p_s3->a)/2);
			p_dp4->b = (uint8_t) (((uint16_t) p_s1->b + (uint16_t) p_s3->b)/2);
			p_dp4->g = (uint8_t) (((uint16_t) p_s1->g + (uint16_t) p_s3->g)/2);
			p_dp4->r = (uint8_t) (((uint16_t) p_s1->r + (uint16_t) p_s3->r)/2);

			p_dp5->a = (uint8_t) (((uint16_t) p_s2->a + (uint16_t) p_s3->a)/2);
			p_dp5->b = (uint8_t) (((uint16_t) p_s2->b + (uint16_t) p_s3->b)/2);
			p_dp5->g = (uint8_t) (((uint16_t) p_s2->g + (uint16_t) p_s3->g)/2);
			p_dp5->r = (uint8_t) (((uint16_t) p_s2->r + (uint16_t) p_s3->r)/2);
		}
	}

	for (unsigned int c = 0; c < dstw; c++) {

		RGBA* p_d0pri = (RGBA*) &dst_matrix[1][c];
		RGBA* p_dest = (RGBA*) &dst_matrix[0][c];

		*p_dest = *p_d0pri;

	}

	for (unsigned int f = 0; f < dsth; f++) {

		RGBA* p_d1pri = (RGBA*) &dst_matrix[f][dstw-2];
		RGBA* p_dest1 = (RGBA*) &dst_matrix[f][dstw-1];

		*p_dest1 = *p_d1pri;

	}
}
