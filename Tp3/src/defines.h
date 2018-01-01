/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

    Definiciones globales del sistema.
*/

#ifndef __DEFINES_H__
#define __DEFINES_H__

/* Bool */
/* -------------------------------------------------------------------------- */
#define TRUE                    0x00000001
#define FALSE                   0x00000000
#define ERROR                   1
#define Pos1					0x8000000
#define Pos2					0x8001000
#define Pos3					0x8002000
#define Pos4					0x8003000
#define Pos5					0x8004000
#define Pos6					0x8005000
#define Pos7					0x8006000
#define Pos8					0x8007000
#define Pos9					0x8008000


/* Misc */
/* -------------------------------------------------------------------------- */
#define CANT_ZOMBIS             8

#define SIZE_W                  78
#define SIZE_H                  44


/* Indices en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_COUNT 31

#define GDT_IDX_NULL_DESC           0
#define GDT_IDX_CODE_0 	            8

/* Offsets en la gdt */
/* -------------------------------------------------------------------------- */
#define GDT_OFF_NULL_DESC           (GDT_IDX_NULL_DESC      << 3)

/* Direcciones de memoria */
/* -------------------------------------------------------------------------- */
#define VIDEO                   0x000B8000 /* direccion fisica del buffer de video */

//#define pantalla_activa 
#define ADDR_BUFFER_VIDEO_ESTADO 		0x000BB000
#define ADDR_BUFFER_VIDEO_MAPA 			0x000BD000

#endif  /* !__DEFINES_H__ */
