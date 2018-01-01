/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#include "mmu.h"
#include "i386.h"
#include "screen.h"
#include "sched.h"

#define PDE_INDEX(dir_lineal) dir_lineal >> 22
#define PTE_INDEX(dir_lineal) dir_lineal >> 12 & (unsigned int) 0x3FF // elimina el offset de la pde

unsigned int CANT_PAGINAS = 0;
unsigned int CANT_PAGINAS_MAPA = 0;
unsigned int page_size = 0x1000; // 4k = 2¹²
unsigned int AREA_LIBRE = 0x100000 ;
unsigned int MAPA = 0x400000 ; 

void mmu_inicializar_dir_kernel() { // kernel

	int i ;
	directory_entry* dir_paginas = (directory_entry*) 0x27000; // porque asi lo pide el tp
	dir_paginas[0] = (directory_entry) {
			(unsigned char) 0x01, 		//present
			(unsigned char) 0x01, 		//read/write
			(unsigned char) 0x00, 		//user1 supervisor0
			(unsigned char) 0x00, 		//page level write through
			(unsigned char) 0x00, 		//page level cache disable
			(unsigned char) 0x00, 		//accessed
			(unsigned char) 0x00, 		//ignored
			(unsigned char) 0x00,		//page_size
			(unsigned char) 0x00, 		//global
			(unsigned char) 0x00, 		//al pedou bits
			(unsigned int) 0x28, 		//page frame
		};
	for (i=1; i < 1024; i++)
	{
		dir_paginas[i] = (directory_entry) {
			(unsigned char) 0x00, 		//present
			(unsigned char) 0x00, 		//read/write
			(unsigned char) 0x00, 		//user supervisor
			(unsigned char) 0x00, 		//page level write through
			(unsigned char) 0x00, 		//page level cache disable
			(unsigned char) 0x00, 		//accessed
			(unsigned char) 0x00, 		//ignored
			(unsigned char) 0x00,		//page_size
			(unsigned char) 0x00, 		//global
			(unsigned char) 0x00, 		//al pedou bits
			(unsigned int) 0x00, 		//page frame
		};
	}

	//inicializando la primer tabla de paginas
	page_entry* paginas = (page_entry*)0x28000; // porque asi lo pide el tp
	for (i=0; i < 1024; i++)
	{
		paginas[i] = (page_entry) {
			(unsigned char) 0x01, 		//present
			(unsigned char) 0x01, 		//read/write
			(unsigned char) 0x00, 		//user supervisor
			(unsigned char) 0x00, 		//page level write through
			(unsigned char) 0x00, 		//page level cache disable
			(unsigned char) 0x00, 		//accessed
			(unsigned char) 0x00, 		//dirty bit
			(unsigned char) 0x00,		//page table attr index
			(unsigned char) 0x00, 		//global
			(unsigned char) 0x00, 		//al pedou bits
			(unsigned int) i, 			//page frame
		};
	}
}

unsigned int proxima_pagina(){
	unsigned int prox_pag = AREA_LIBRE + (CANT_PAGINAS * page_size);
	CANT_PAGINAS++;
	return prox_pag;
}

unsigned int proxima_pagina_mapa(){
	unsigned int prox_pag = MAPA + (CANT_PAGINAS_MAPA * page_size);
	CANT_PAGINAS_MAPA++;
	return prox_pag;
}




void mmu_mapear_nueva_pagina(unsigned int dir_lineal , unsigned int cr3, unsigned int dir_fisica, unsigned int user_sup, unsigned int read_write){

	directory_entry* dir_paginas = (directory_entry*) cr3;
	unsigned short pde_index = PDE_INDEX(dir_lineal);
	unsigned short pte_index = PTE_INDEX(dir_lineal);

	if (dir_paginas[pde_index].present == 0)
	{
		unsigned int pagina = proxima_pagina();
		inicializar_page_dir(pagina);
		dir_paginas[pde_index] = (directory_entry) {
			(unsigned char) 0x01, 			//present
			(unsigned char) read_write, 	//read/write
			(unsigned char) user_sup, 		//user supervisorº
			(unsigned char) 0x00, 			//page level write through
			(unsigned char) 0x00, 			//page level cache disable
			(unsigned char) 0x00, 			//accessed
			(unsigned char) 0x00, 			//ignored
			(unsigned char) 0x00,			//page_size
			(unsigned char) 0x00, 			//global
			(unsigned char) 0x00, 			//en blanco
			(unsigned int) pagina >> 12, 	//page frame
		};
	}
	
	page_entry* table_paginas = (page_entry*) ((dir_paginas[pde_index]).page_address << 12);

	//if (table_paginas[pte_index].present == 0)
	//{
		table_paginas[pte_index] = (page_entry) {
			(unsigned char) 0x01, 				//present
			(unsigned char) read_write, 		//read/write
			(unsigned char) user_sup, 			//user supervisor
			(unsigned char) 0x00, 				//page level write through
			(unsigned char) 0x00, 				//page level cache disable
			(unsigned char) 0x00, 				//accessed
			(unsigned char) 0x00, 				//dirty
			(unsigned char) 0x00,				//page_attribute_index
			(unsigned char) 0x00, 				//global
			(unsigned char) 0x00, 				//en blanco
			(unsigned int) dir_fisica >> 12, 	//page frame
		};
	//}
	//breakpoint();
	tlbflush();
}



void mmu_unmapear_pagina(unsigned int dir_lineal , unsigned int cr3){
	directory_entry* dir_paginas = (directory_entry*) cr3 ;

	unsigned short pde_index = PDE_INDEX(dir_lineal);

	if (dir_paginas[pde_index].present == 1){


		unsigned short pte_index = PTE_INDEX(dir_lineal);
		page_entry* table_paginas = (page_entry*) ((dir_paginas[pde_index]).page_address << 12);
		if (table_paginas[pte_index].present == 1)
		{
			table_paginas[pte_index] = (page_entry) {
				(unsigned char) 0x00, 		//present
				(unsigned char) 0x00, 		//read/write
				(unsigned char) 0x00, 		//user supervisor
				(unsigned char) 0x00, 		//page level write through
				(unsigned char) 0x00, 		//page level cache disable
				(unsigned char) 0x00, 		//accessed
				(unsigned char) 0x00, 		//dirty
				(unsigned char) 0x00,		//page_attribute_index
				(unsigned char) 0x00, 		//global
				(unsigned char) 0x00, 		//al pedou bits
				(unsigned int)	0x00, 		//page frame
			};
		}

	}

}

unsigned int mmu_inicializar_dir_zombi(unsigned int jugador, unsigned int x, unsigned int y, unsigned int tarea) {

	directory_entry* directorio = (directory_entry*) proxima_pagina() ;
	
	mmu_inicializar_directorio((unsigned int)directorio); //kernel
	//breakpoint();

	game_nueva_pos((unsigned int)directorio,x,y);
	//breakpoint();
	//jugador 1 = 0
	// j1 guerrero = 0
	// j1 mago = 1
	// j1 clerigo = 2

	//jugador 2 = 3
	// j2 guerrero = 0
	// j2 mago = 1
	// j2 clerigo = 2
	unsigned int direccion_tarea = 0;

	if (jugador == 0){
	direccion_tarea = s.tareasA[tarea].codigo;
	}
	else
	{
	direccion_tarea = s.tareasB[tarea].codigo;
	}

    mmu_mapear_nueva_pagina(pos2map(x,y), (unsigned int) rcr3(), pos2map(x,y), 1, 1);
	copiar_pagina((unsigned int*) direccion_tarea, (unsigned int*) pos2map(x,y));
	mmu_unmapear_pagina(pos2map(x,y), (unsigned int) rcr3() );
	return  (unsigned int) directorio;

}



void copiar_pagina(unsigned int* src ,unsigned int* dst){

	int i ;
	for(i=0;i<page_size/4;i++){
		dst[i] = src[i];
	}
}

unsigned int averiguar_dir_fisica(unsigned int cr3, unsigned int dir_lineal) {
	directory_entry* dir_paginas = (directory_entry*) cr3 ;

	unsigned short pde_index = PDE_INDEX(dir_lineal);

	unsigned short pte_index = PTE_INDEX(dir_lineal);
	page_entry* table_paginas = (page_entry*) ((dir_paginas[pde_index]).page_address << 12);
	
	return (table_paginas[pte_index].page_address << 12);
}







void inicializar_page_dir(unsigned int pagina) { // generica, en 0
	
	int i;
	directory_entry* dir_paginas = (directory_entry*) pagina;
	//breakpoint();
	for (i=0; i < 1024; i++)
	{
		dir_paginas[i] = (directory_entry) {
			(unsigned char) 0x00, 		//present
			(unsigned char) 0x00, 		//read/write
			(unsigned char) 0x00, 		//user supervisor
			(unsigned char) 0x00, 		//page level write through
			(unsigned char) 0x00, 		//page level cache disable
			(unsigned char) 0x00, 		//accessed
			(unsigned char) 0x00, 		//ignored
			(unsigned char) 0x00,		//page_size
			(unsigned char) 0x00, 		//global
			(unsigned char) 0x00, 		//al pedou bits
			(unsigned int) 0x00, 			//page frame
		};
		//breakpoint();
	}
}

void inicializar_tabla(unsigned int pagina) { // generica, en 0

int i;
	page_entry* table_paginas = (page_entry*) pagina;
	for (i=0; i < 1024; i++)
	{
		table_paginas[i] = (page_entry) {
			(unsigned char) 0x00, 		//present
			(unsigned char) 0x00, 		//read/write
			(unsigned char) 0x00, 		//user supervisor
			(unsigned char) 0x00, 		//page level write through
			(unsigned char) 0x00, 		//page level cache disable
			(unsigned char) 0x00, 		//accessed
			(unsigned char) 0x00, 		//dirty
			(unsigned char) 0x00,		//page_attribute_index
			(unsigned char) 0x00, 		//global
			(unsigned char) 0x00, 		//al pedou bits
			(unsigned int) 0x00, 		//page frame
		};

	}
}


unsigned int pos2map(unsigned int x, unsigned int y){
	return (0x400000 + ((x + y*SIZE_W))*page_size);
}

unsigned int map2posx(unsigned int dir){
	//breakpoint();
	return dir / (SIZE_W*page_size);
}

unsigned int map2posy(unsigned int dir){
	return dir % (SIZE_W*page_size);
}



void mmu_inicializar_directorio(unsigned int directorio) {
	//inicializar directorio de paginas
	int i ;
	directory_entry* dir_paginas = (directory_entry*) directorio;
	dir_paginas[0] = (directory_entry) {
			(unsigned char) 0x01, 		//present
			(unsigned char) 0x01, 		//read/write
			(unsigned char) 0x00, 		//user supervisor
			(unsigned char) 0x00, 		//page level write through
			(unsigned char) 0x00, 		//page level cache disable
			(unsigned char) 0x00, 		//accessed
			(unsigned char) 0x00, 		//ignored
			(unsigned char) 0x00,		//page_size
			(unsigned char) 0x00, 		//global
			(unsigned char) 0x00, 		//al pedou bits
			(unsigned int) 0x28, 		//page frame
		};

	for (i=1; i < 1024; i++)
	{
		dir_paginas[i] = (directory_entry) {
			(unsigned char) 0x00, 		//present
			(unsigned char) 0x00, 		//read/write
			(unsigned char) 0x00, 		//user supervisor
			(unsigned char) 0x00, 		//page level write through
			(unsigned char) 0x00, 		//page level cache disable
			(unsigned char) 0x00, 		//accessed
			(unsigned char) 0x00, 		//ignored
			(unsigned char) 0x00,		//page_size
			(unsigned char) 0x00, 		//global
			(unsigned char) 0x00, 		//al pedou bits
			(unsigned int) 0x00,	 	//page frame
		};

	}
	//inicializando la primer tabla de paginas

	page_entry* paginas = (page_entry*) proxima_pagina();
	for (i=0; i < 1024; i++)
	{
		paginas[i] = (page_entry) {
			(unsigned char) 0x01, 		//present
			(unsigned char) 0x01, 		//read/write
			(unsigned char) 0x00, 		//user supervisor
			(unsigned char) 0x00, 		//page level write through
			(unsigned char) 0x00, 		//page level cache disable

			(unsigned char) 0x00, 		//accessed
			(unsigned char) 0x00, 		//dirty bit
			(unsigned char) 0x00,		//page table attr index
			(unsigned char) 0x00, 		//global
			(unsigned char) 0x00, 		//al pedou bits
			(unsigned int) i, 			//page frame
		};

	}

}