/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCHED_H__
#define __SCHED_H__

#include "screen.h"
#include "tss.h"




typedef struct str_sched_entry_fld {
    unsigned short punterogdt; // puntero de la gdt del zombie actual
	unsigned int activo; // 1 vivo=activo, 0 muerto=desactivo
	unsigned int posx;
	unsigned int posy;
  unsigned int tipoZombie;
  unsigned int reloj;
  unsigned int codigo;
} sched_entry;


typedef struct str_scheduler_fld {

   struct str_sched_entry_fld tareasA[8];
   struct str_sched_entry_fld tareasB[8];
   unsigned int indice_zombie_actualA; // posicion en el arreglo de 0 a 7
   unsigned int indice_zombie_actualB; // posicion en el arreglo de 0 a 7
} sched;



unsigned short sched_proximo_indice();
void sched_inicializar();
void sched_agregar(unsigned int jugador, unsigned int x, unsigned int y);
void sched_sacar(unsigned int jugador, unsigned int tarea);
void llamar_tlbflush();
void actualizar_pos(unsigned int jugador,unsigned int tarea, unsigned int x, unsigned int y);

unsigned int dame_pos_actualx(unsigned int jugador,unsigned int tarea);
unsigned int dame_pos_actualy(unsigned int jugador,unsigned int tarea);


sched s;
extern unsigned int jugador_actual; // 0 es A, 1 es B
extern unsigned int cantidad_zombiesA;
extern unsigned int cantidad_zombiesB;
extern unsigned int puntosA;
extern unsigned int puntosB;
extern unsigned int tareas_vivas;



#endif	/* !__SCHED_H__ */
