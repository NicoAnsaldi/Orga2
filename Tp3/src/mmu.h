/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#ifndef __MMU_H__
#define __MMU_H__

#include "defines.h"
#include "i386.h"
#include "tss.h"
#include "game.h"


extern unsigned int proxima_pagina();
extern unsigned int proxima_pagina_mapa();
extern void mmu_inicializar_dir_kernel();
extern unsigned int mmu_inicializar_dir_zombi(unsigned int jugador, unsigned int x, unsigned int y, unsigned int tarea);
extern void mmu_inicializar_directorio(unsigned int directorio);
extern void inicializar_page_dir(unsigned int pagina);
extern void inicializar_tabla(unsigned int pagina);
extern void mmu_mapear_nueva_pagina(unsigned int dir_lineal , unsigned int cr3, unsigned int dir_fisica, unsigned int user_sup, unsigned int read_write);
extern void mmu_unmapear_pagina(unsigned int dir_lineal , unsigned int cr3);
extern void copiar_pagina(unsigned int* dir, unsigned int* val);
extern unsigned int averiguar_dir_fisica(unsigned int cr3, unsigned int dir_lineal);
extern unsigned int pos2map(unsigned int x, unsigned int y);
unsigned int map2posx(unsigned int dir);
unsigned int map2posy(unsigned int dir);

typedef struct str_directory_entry {
    unsigned char   present:1;
    unsigned char   r_w:1;
    unsigned char   user_sup:1;
    unsigned char   page_level_write_through:1;
    unsigned char   page_level_cache_disable:1;
    unsigned char   accessed:1;
    unsigned char   ignored:1;
    unsigned char   page_size:1;
    unsigned char   global:1;
    unsigned char   al_pedou_bits:3;
    unsigned int   page_address:20;
} __attribute__((__packed__, aligned (4))) directory_entry;


typedef struct str_page_entry {
    unsigned char   present:1;
    unsigned char   r_w:1;
    unsigned char   user_sup:1;
    unsigned char   page_level_write_through:1;
    unsigned char   page_level_cache_disable:1;
    unsigned char   accessed:1;
    unsigned char   dirty:1;
    unsigned char   page_table_attr_index:1;
    unsigned char   global:1;
    unsigned char   al_pedou_bits:3;
    unsigned int   page_address:20;
} __attribute__((__packed__, aligned (4))) page_entry;


extern unsigned int page_size;
#endif	/* !__MMU_H__ */




