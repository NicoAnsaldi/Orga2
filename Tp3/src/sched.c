/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/


#include "sched.h"
#include "i386.h"
#include "screen.h"
#include "tss.h"
#include "mmu.h"
#include "game.h"


#define IDLE 14<<3

unsigned int jugador_actual = 0;
unsigned int cantidad_zombiesA = 0;
unsigned int cantidad_zombiesB = 0;
unsigned int puntosA = 0;
unsigned int puntosB = 0;
unsigned int haytareasA = 0;
unsigned int haytareasB = 0;

void llamar_tlbflush() {
	tlbflush();
}

void actualizar_pos(unsigned int jugador,unsigned int tarea, unsigned int x, unsigned int y) {

	if (jugador == 0){
		s.tareasA[tarea].posx = x;
		s.tareasA[tarea].posy = y;
	}
	else
	{
		s.tareasB[tarea].posx = x;
		s.tareasB[tarea].posy = y;
	}
}

unsigned int dame_pos_actualx(unsigned int jugador,unsigned int tarea){
	if (jugador == 0){
		return s.tareasA[tarea].posx;
	}
	else
	{
		return s.tareasB[tarea].posx;
	}
}

unsigned int dame_pos_actualy(unsigned int jugador,unsigned int tarea){
	if (jugador == 0){
		return s.tareasA[tarea].posy;
	}
	else
	{
		return s.tareasB[tarea].posy;
	}
}

void sched_inicializar() {

	int i;
	for(i=0; i< 8; i++){

			s.tareasA[i] = (sched_entry){
				(unsigned short) ((15+i)<<3),
				0,
				0,
				0,
				0,
				0,
				0x10000,
			};

			s.tareasB[i] = (sched_entry){
				(unsigned short) ((23+i)<<3),
				0,
				0,
				0,
				0,
				0,
				0x13000,
			};
	}	
		s.indice_zombie_actualA = 0;
		s.indice_zombie_actualB = 0;
}

unsigned short sched_proximo_indice() {

	if (jugador_actual == 0)
	{ 
		unsigned int i = s.indice_zombie_actualA+1; // puede ser 8
		if (i == 8) i = 0;

		while(i != s.indice_zombie_actualA && !(s.tareasA[i].activo)){
			i++;
			if (i == 8) i = 0;
		}

		if(s.tareasA[i].activo)
		{
			s.indice_zombie_actualA = i;
			//print_int(i,30+i,0,C_BG_RED + C_FG_WHITE);
			return s.tareasA[i].punterogdt;
		}
		return 20;
	}
	else
	{
		unsigned int i = s.indice_zombie_actualB+1; // puede ser 8
		if (i == 8) i = 0;
	 
	 	while(i != s.indice_zombie_actualB && !(s.tareasB[i].activo)){
			i++;
			if (i == 8) i = 0;
		}
		
		if(s.tareasB[i].activo){
			//breakpoint();
			s.indice_zombie_actualB = i;
			return s.tareasB[i].punterogdt;
		}
		return 20;
	}
}

void sched_agregar(unsigned int jugador, unsigned int x, unsigned int y){
//breakpoint();
	unsigned int la = tipoZombieA;
	unsigned int lb = tipoZombieB;
	unsigned int i;
	if (jugador == 0){

		if(haytareasA == 0){
			i = 0;
			s.indice_zombie_actualA = 0;
			//print_int(i,50+i,0,C_BG_BLACK + C_FG_WHITE);
			s.tareasA[0].punterogdt = (unsigned short) ((15)<<3);
			s.tareasA[0].activo = 1;
			haytareasA = 1;
			s.tareasA[i].tipoZombie = la;
			s.tareasA[i].codigo = 0x10000 + ( (s.tareasA[i].tipoZombie) * page_size);
			reset_tss(0, jugador, x ,y);
		}
		else
		{
			i = s.indice_zombie_actualA+1;
			if (i == 8) i = 0;

			while(i != s.indice_zombie_actualA && (s.tareasA[i].activo)){
				i++;
				if (i == 8) i = 0;
			}// o i es el actual y no encontra inactiva o encontre alguna inactiva
			s.indice_zombie_actualA = i;
			//print_int(i,50+i,0,C_BG_BLACK + C_FG_WHITE);
			s.tareasA[i].punterogdt = (unsigned short) ((15+i)<<3);
			
			s.tareasA[i].activo = 1;
			s.tareasA[i].tipoZombie = la;
			s.tareasA[i].codigo = 0x10000 + ( (s.tareasA[i].tipoZombie) * page_size);
			reset_tss(i, jugador, x ,y);
		}
	}
	else
	{
		if(haytareasB == 0){
			i = 0;
			s.indice_zombie_actualB = 0;
			//print_int(i,50+i,0,C_BG_BLACK + C_FG_WHITE);
			s.tareasB[0].punterogdt = (unsigned short) ((23)<<3);
			s.tareasB[0].activo = 1;
			haytareasB = 1;
			s.tareasB[i].tipoZombie = lb;
			s.tareasB[i].codigo = 0x13000 + ( (s.tareasB[i].tipoZombie) * page_size);
			reset_tss(0, jugador, x ,y);
		}
		else
		{
			i = s.indice_zombie_actualB+1;
			if (i == 8) i = 0;

			while(i != s.indice_zombie_actualB && (s.tareasB[i].activo)){
				i++;
				if (i == 8) i = 0;
			}// o i es el actual y no encontra inactiva o encontre alguna inactiva
			s.indice_zombie_actualB = i;
			//print_int(i,50+i,0,C_BG_BLACK + C_FG_WHITE);
			s.tareasB[i].punterogdt = (unsigned short) ((23+i)<<3);
			s.tareasB[i].activo = 1;
			s.tareasB[i].tipoZombie = lb;
			s.tareasB[i].codigo = 0x13000 + ( (s.tareasB[i].tipoZombie) * page_size);
			reset_tss(i, jugador, x ,y);
		}
	}
}

void sched_sacar(unsigned int jugador, unsigned int tarea)
{
	//breakpoint();
	if(jugador == 0)
	{
   		s.tareasA[tarea].activo = 0;
   		cantidad_zombiesA--;
   		print_mapa("X",s.tareasA[tarea].posx, s.tareasA[tarea].posy, C_BG_GREEN + C_FG_RED);
   	}
	else
	{
   		s.tareasB[tarea].activo = 0;
   		cantidad_zombiesB--;
   		print_mapa("X",s.tareasB[tarea].posx, s.tareasB[tarea].posy, C_BG_GREEN + C_FG_RED);
   	}
}

//si se rompe, desalojarla
//ponerlo en el estado, bah hacer el estado