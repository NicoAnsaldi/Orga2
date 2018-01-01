/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#ifndef __SCREEN_H__
#define __SCREEN_H__

/* Definicion de la pantalla */
#define VIDEO_FILS 50
#define VIDEO_COLS 80

#include "colors.h"
#include "defines.h"
/* Estructura de para acceder a memoria de video */
typedef struct ca_s {
    unsigned char c;
    unsigned char a;
} ca;


void print_hex(unsigned int numero, int size, unsigned int x, unsigned int y, unsigned short attr);
void print_int(unsigned int n, unsigned int x, unsigned int y, unsigned short attr);
//void print_isr(unsigned int n, unsigned short task, unsigned int fila);

void iniciar_pantalla_mapa();
void iniciar_estado();
void memcopy(int dst, int src, int size);

void print_mapa(const char * text, unsigned int x, unsigned int y, unsigned short attr);
void print_Zombie(unsigned int n, unsigned int x,  unsigned int y, unsigned short attr);
void print_teclado(const char * text, unsigned int x, unsigned int y, unsigned short attr);

void cambiar_tipoA(unsigned int tipo );
void cambiar_tipoB(unsigned int tipo );
void cambiar_posicionA(unsigned int pos );
void cambiar_posicionB(unsigned int pos );

void donde_estoy(unsigned int jugador, unsigned int tarea);
void donde_estaba(unsigned int jugador, unsigned int tarea);
void print_puntos();
void print_tss(unsigned int errorcode, unsigned int eip, unsigned int cs, unsigned int eflags, unsigned int esp, unsigned int ss);
void mostrar_mapa();
void estoy_en_estado();

extern int tipoZombieA; // 0, 1 , 2
extern int tipoZombieB;

extern int posicionA;
extern int posicionB;

int cr0_e;
int cr2_e;
int cr4_e;

int stack1;
int stack2;
int stack3;
int stack4;
int stack5;

#endif  /* !__SCREEN_H__ */
