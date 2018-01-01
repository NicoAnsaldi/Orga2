/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"
#include "sched.h"
#include "screen.h"
#include "mmu.h"
#include "i386.h"

	unsigned int zombies20A = 0;
	unsigned int zombies20B = 0;
	unsigned int nopiseZombieA = 1;
	unsigned int nopiseZombieB = 1;
	unsigned int modoDebug = 0;
	unsigned int modoDebugFueActivado = 0;

void game_nueva_pos(unsigned int cr3, unsigned int x, unsigned int y) {

	if(jugador_actual == 0){
	mmu_mapear_nueva_pagina(Pos7, cr3, pos2map(x-1,dentro_rango(y)), 1, 1);
	mmu_mapear_nueva_pagina(Pos1, cr3, pos2map(x,dentro_rango(y)), 1, 1);
	mmu_mapear_nueva_pagina(Pos2, cr3, pos2map(x+1,dentro_rango(y)), 1, 1);

	mmu_mapear_nueva_pagina(Pos9, cr3, pos2map(x-1,dentro_rango(y+1)), 1, 1);
	mmu_mapear_nueva_pagina(Pos5, cr3, pos2map(x,dentro_rango(y+1)), 1, 1);
	mmu_mapear_nueva_pagina(Pos3, cr3, pos2map(x+1,dentro_rango(y+1)), 1, 1);

	mmu_mapear_nueva_pagina(Pos8, cr3, pos2map(x-1,dentro_rango(y-1)), 1, 1);
	mmu_mapear_nueva_pagina(Pos6, cr3, pos2map(x,dentro_rango(y-1)), 1, 1);
	mmu_mapear_nueva_pagina(Pos4, cr3, pos2map(x+1,dentro_rango(y-1)), 1, 1);
	}
	else
	{
	mmu_mapear_nueva_pagina(Pos7, cr3, pos2map(x+1,dentro_rango(y)), 1, 1);
	mmu_mapear_nueva_pagina(Pos1, cr3, pos2map(x,dentro_rango(y)), 1, 1);
	mmu_mapear_nueva_pagina(Pos2, cr3, pos2map(x-1,dentro_rango(y)), 1, 1);

	mmu_mapear_nueva_pagina(Pos9, cr3, pos2map(x+1,dentro_rango(y-1)), 1, 1);
	mmu_mapear_nueva_pagina(Pos5, cr3, pos2map(x,dentro_rango(y-1)), 1, 1);
	mmu_mapear_nueva_pagina(Pos3, cr3, pos2map(x-1,dentro_rango(y-1)), 1, 1);

	mmu_mapear_nueva_pagina(Pos8, cr3, pos2map(x+1,dentro_rango(y+1)), 1, 1);
	mmu_mapear_nueva_pagina(Pos6, cr3, pos2map(x,dentro_rango(y+1)), 1, 1);
	mmu_mapear_nueva_pagina(Pos4, cr3, pos2map(x-1,dentro_rango(y+1)), 1, 1);
	}
}

unsigned int dentro_rango(unsigned int x){
	if( x == 45) return 1;
	if( x == 0) return 44;
	return x;
}

void game_jugador_mover(unsigned int value) {
	unsigned int cr3 =  rcr3();
	int tareaA = s.indice_zombie_actualA;
	int tareaB = s.indice_zombie_actualB;
	int jugador = jugador_actual;

   	if (jugador == 0){
   		int x = dame_pos_actualx(jugador,tareaA);
		int y = dame_pos_actualy(jugador,tareaA);

		if (nopiseZombieA){
			donde_estaba(jugador,tareaA);
		} 
		nopiseZombieA = 1;

		int src;

		if (value == IZQ){
 			src = Pos5;
			y = y-1;
		}
		if (value == DER){
			src = Pos6;
			y = y+1;
		}
		if (value == ADE){
			src = Pos7;
			x = x+1;
		}
		if (value == ATR){
			src = Pos2;
			x = x-1;
		}
		//breakpoint();
		game_nueva_pos(cr3,x,y);
		copiar_pagina((unsigned int*) src, (unsigned int*) Pos1);

		actualizar_pos(jugador,tareaA, x, dentro_rango(y));
		donde_estoy(jugador,tareaA);

		if( x == 1){
			print_mapa("X",x, dentro_rango(y), C_BG_GREEN + C_FG_RED);
			sched_sacar(jugador,tareaA);
			puntosB++;
			print_puntos();
		} 

		if( x == 78){
			print_mapa("X",x, dentro_rango(y), C_BG_GREEN + C_FG_RED);
			sched_sacar(jugador,tareaA);
			puntosA++;
			print_puntos();
		} 
	}
	else // jugador B
	{
		int x = dame_pos_actualx(jugador,tareaB);
		int y = dame_pos_actualy(jugador,tareaB);

		if (nopiseZombieB){
			donde_estaba(jugador,tareaB);
		} 
		nopiseZombieB = 1;

		int src;

		if (value == IZQ){
 			src = Pos5;
			y = y+1;
		}
		if (value == DER){
			src = Pos6;
			y = y-1;
		}
		if (value == ADE){
			src = Pos7;
			x = x-1;
		}
		if (value == ATR){
			src = Pos2;
			x = x+1;
		}
		//breakpoint();
		game_nueva_pos(cr3,x,y);
		copiar_pagina((unsigned int*) src, (unsigned int*) Pos1);

		actualizar_pos(jugador,tareaB, x, dentro_rango(y));
		donde_estoy(jugador,tareaB);

		if( x == 1){
			print_mapa("X",x, dentro_rango(y), C_BG_GREEN + C_FG_RED);
			sched_sacar(jugador,tareaB);
			puntosB++;
			print_puntos();
		} 

		if( x == 78){
			print_mapa("X",x, dentro_rango(y), C_BG_GREEN + C_FG_RED);
			sched_sacar(jugador,tareaB);
			puntosA++;
			print_puntos();
		} 
	}
}




void game_lanzar_zombi(unsigned int jugador) {
	//if (cantidad_zombiesB == 7) breakpoint();
//breakpoint();
	if (puntosA < 10 && puntosB < 10){
		int x;
		int y;
		jugador_actual = jugador;

		if (jugador == 0){
			if(zombies20A < 20 && cantidad_zombiesA < 8){
				x = 2;
				y = posicionA;
				sched_agregar(jugador,x, y);
				actualizar_pos(jugador,s.indice_zombie_actualA, x, y);
				donde_estoy(jugador, s.indice_zombie_actualA);
				cantidad_zombiesA++;
				zombies20A++;
			}
		}
		else
		{
			if(zombies20B < 20 && cantidad_zombiesB < 8){
				x = 77;
				y = posicionB;
				sched_agregar(jugador, x, y);
				actualizar_pos(jugador,s.indice_zombie_actualB, x, y);
				donde_estoy(jugador, s.indice_zombie_actualB);
				cantidad_zombiesB++;
				zombies20B++;
			}
		}
	}
	//print_int(zombies20B,10,10,C_BG_GREEN + C_FG_RED);
	//print_int(cantidad_zombiesB,14,10,C_BG_GREEN + C_FG_BLUE);

}


void cambiar_jugador() {
	if (jugador_actual == 0){
		jugador_actual = 1;
	}
	else
	{
		jugador_actual = 0;
	}
}



