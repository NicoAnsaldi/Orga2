/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#ifndef __GAME_H__
#define __GAME_H__

#include "defines.h"
#include "screen.h"
#include "mmu.h"

typedef enum direccion_e { IZQ = 0xAAA, DER = 0x441, ADE = 0x83D, ATR = 0x732 } direccion;

void  game_jugador_mover(unsigned int value);

void game_lanzar_zombi(unsigned int jugador);
void cambiar_jugador();

void game_nueva_pos(unsigned int cr3,unsigned int x,unsigned int y);

unsigned int dentro_rango(unsigned int x);

typedef struct posicion {
    unsigned int x; 
	unsigned int y; 
} pos;


unsigned int zombies20A;
unsigned int zombies20B;
unsigned int modoDebug;
unsigned int modoDebugFueActivado;


#endif  /* !__GAME_H__ */
