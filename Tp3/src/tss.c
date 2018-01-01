/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de estructuras para administrar tareas
*/

#include "tss.h"
#include "screen.h"
#include "mmu.h"

tss tss_inicial;
tss tss_idle;

tss tss_zombisA[CANT_ZOMBIS];
tss tss_zombisB[CANT_ZOMBIS];

void tss_inicializar() {

    tss_inicial.ptl = 0;
    tss_inicial.unused0 = 0;
    tss_inicial.esp0 = 0;
    tss_inicial.ss0 = 0;
    tss_inicial.unused1 = 0;
    tss_inicial.esp1 = 0;
    tss_inicial.ss1 = 0;
    tss_inicial.unused2 = 0;
    tss_inicial.esp2 = 0;
    tss_inicial.ss2 = 0;
    tss_inicial.unused3 = 0;
    tss_inicial.cr3 = 0;
    tss_inicial.eip = 0; 
    tss_inicial.eflags = 0;    
    tss_inicial.eax = 0;
    tss_inicial.ecx = 0;
    tss_inicial.edx = 0;
    tss_inicial.ebx = 0;
    tss_inicial.esp = 0; 
    tss_inicial.ebp = 0; 
    tss_inicial.esi = 0;
    tss_inicial.edi = 0;
    tss_inicial.es = 0; // datos
    tss_inicial.unused4 = 0;
    tss_inicial.cs = 0; // codigo
    tss_inicial.unused5 = 0;
    tss_inicial.ss = 0;
    tss_inicial.unused6 = 0;
    tss_inicial.ds = 0;
    tss_inicial.unused7 = 0;
    tss_inicial.fs = 0; // video
    tss_inicial.unused8 = 0;
    tss_inicial.gs =0;
    tss_inicial.unused9 = 0;
    tss_inicial.ldt = 0;
    tss_inicial.unused10 = 0;
    tss_inicial.dtrap = 0;
    tss_inicial.iomap = 0;


//IDLE
	tss_idle.ptl = 0; // aca no va nada, por las tareas anidadas
    tss_idle.unused0 = 0;
    tss_idle.esp0 = 0; 
    tss_idle.ss0 = 0; 
    tss_idle.unused1 = 0;
    tss_idle.esp1 = 0;
    tss_idle.ss1 = 0;
    tss_idle.unused2 = 0;
    tss_idle.esp2 = 0;
    tss_idle.ss2 = 0;
    tss_idle.unused3 = 0;
    tss_idle.cr3 = (int) 0x27000; // comparte el pde con el kernel
    tss_idle.eip = (int) 0x16000; // en 16000 esta el cogido de la idle
	tss_idle.eflags = (int) 0x202;	// permite interrupciones

	tss_idle.eax = 0;
    tss_idle.ecx = 0;
    tss_idle.edx = 0;
    tss_idle.ebx = 0;
    tss_idle.esp = (int) 0x00027000; // PILA DEL KERNEL
    tss_idle.ebp = (int) 0x00027000;
    tss_idle.esi = 0;
    tss_idle.edi = 0;

    tss_idle.es = (int) 9<<3; // datos
    tss_idle.unused4 = 0;
    tss_idle.cs = (int) 8<<3; // codigo
    tss_idle.unused5 = 0;
    tss_idle.ss = (int) 9<<3;
    tss_idle.unused6 = 0;
    tss_idle.ds = (int) 9<<3;
    tss_idle.unused7 = 0;
    tss_idle.fs = (int) 12<<3; // video
    tss_idle.unused8 = 0;
    tss_idle.gs =(int) 9<<3;
    tss_idle.unused9 = 0;
    tss_idle.ldt = 0;
    tss_idle.unused10 = 0;
    tss_idle.dtrap = 0;
    tss_idle.iomap = (int) 0xFFFF; // aca no va nada



    //ZOMBIEESSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS!


    int i;
    for(i = 0; i < 8 ; i++)
    {
    //ZOMBIE A i
    tss_zombisA[i].ptl = 0;
    tss_zombisA[i].unused0 = 0;
    unsigned int stack0_pointer = (unsigned int) proxima_pagina();
    tss_zombisA[i].esp0 = (unsigned int ) stack0_pointer + 0x1000;
    tss_zombisA[i].ss0 = (unsigned short) (9<<3) ; // datos lvl 0
    tss_zombisA[i].unused1 = 0;
    tss_zombisA[i].esp1 = 0;
    tss_zombisA[i].ss1 = 0;
    tss_zombisA[i].unused2 = 0;
    tss_zombisA[i].esp2 = 0;
    tss_zombisA[i].ss2 = 0;
    tss_zombisA[i].unused3 = 0;
 

    tss_zombisA[i].cr3 = (int) 0;

    tss_zombisA[i].eip = (int) 0x8000000;
	tss_zombisA[i].eflags = (int) 0x202;	

	tss_zombisA[i].eax = 0;
    tss_zombisA[i].ecx = 0;
    tss_zombisA[i].edx = 0;
    tss_zombisA[i].ebx = 0;
    tss_zombisA[i].esp = (int) 0x8001000; // apunta a la base de la pila, que por el tp esta abajo de la pagina de codigo (virtual)
    tss_zombisA[i].ebp = (int) 0x8001000; 
    tss_zombisA[i].esi = 0;
    tss_zombisA[i].edi = 0;

    tss_zombisA[i].es = (unsigned short) (11<<3) | 0x3; //  PORQUE ASI = porque el dpl es 3 entonces hay que ponerlo en 3.
    tss_zombisA[i].unused4 = 0;
    tss_zombisA[i].cs = (unsigned short) (10<<3) | 0x3; //10 codigo 11 dato
    tss_zombisA[i].unused5 = 0;
    tss_zombisA[i].ss = (unsigned short) (11<<3) | 0x3;
    tss_zombisA[i].unused6 = 0;
    tss_zombisA[i].ds = (unsigned short) (11<<3) | 0x3;
    tss_zombisA[i].unused7 = 0;
    tss_zombisA[i].fs = (unsigned short) (11<<3) | 0x3;
    tss_zombisA[i].unused8 = 0;
    tss_zombisA[i].gs =(unsigned short) (11<<3) | 0x3; 
    tss_zombisA[i].unused9 = 0;
    tss_zombisA[i].ldt = 0;
    tss_zombisA[i].unused10 = 0;
    tss_zombisA[i].dtrap = 0;
    tss_zombisA[i].iomap = (int) 0xFFFF;

    }



    for(i = 0; i < 8 ; i++)
    {
    	//ZOMBIE B i
    tss_zombisB[i].ptl = 0;
    tss_zombisB[i].unused0 = 0;
    unsigned int stack0_pointer = (unsigned int) proxima_pagina();
    tss_zombisB[i].esp0 = (unsigned int ) stack0_pointer + 0x1000;
    tss_zombisB[i].ss0 = (unsigned short) (9<<3) ;
    tss_zombisB[i].unused1 = 0;
    tss_zombisB[i].esp1 = 0;
    tss_zombisB[i].ss1 = 0;
    tss_zombisB[i].unused2 = 0;
    tss_zombisB[i].esp2 = 0;
    tss_zombisB[i].ss2 = 0;
    tss_zombisB[i].unused3 = 0;
 
                                        
    tss_zombisB[i].cr3 = (int) 0;
    tss_zombisB[i].eip = (int) 0x8000000;
	tss_zombisB[i].eflags = (int) 0x202;	

	tss_zombisB[i].eax = 0;
    tss_zombisB[i].ecx = 0;
    tss_zombisB[i].edx = 0;
    tss_zombisB[i].ebx = 0;
    tss_zombisB[i].esp = (int) 0x8001000;
    tss_zombisB[i].ebp = (int) 0x8001000;
    tss_zombisB[i].esi = 0;
    tss_zombisB[i].edi = 0;

    tss_zombisB[i].es = (unsigned short) (11<<3) | 0x3;
    tss_zombisB[i].unused4 = 0;
    tss_zombisB[i].cs = (unsigned short) (10<<3) | 0x3;
    tss_zombisB[i].unused5 = 0;
    tss_zombisB[i].ss = (unsigned short) (11<<3) | 0x3;
    tss_zombisB[i].unused6 = 0;
    tss_zombisB[i].ds = (unsigned short) (11<<3) | 0x3;
    tss_zombisB[i].unused7 = 0;
    tss_zombisB[i].fs = (unsigned short) (11<<3) | 0x3;
    tss_zombisB[i].unused8 = 0;
    tss_zombisB[i].gs = (unsigned short) (11<<3) | 0x3; 
    tss_zombisB[i].unused9 = 0;
    tss_zombisB[i].ldt = 0;
    tss_zombisB[i].unused10 = 0;
    tss_zombisB[i].dtrap = 0;
    tss_zombisB[i].iomap = (int) 0xFFFF;

    }
}

void reset_tss(unsigned short i, unsigned int jugador, unsigned int x, unsigned int y) {
    
    if (jugador == 0){
        //breakpoint();
        tss_zombisA[i].cr3 = (unsigned int ) mmu_inicializar_dir_zombi(0,x,y,i);                                 
        tss_zombisA[i].eip = (int) 0x8000000;
        tss_zombisA[i].eflags = (int) 0x202;    

        tss_zombisA[i].eax = 0;
        tss_zombisA[i].ecx = 0;
        tss_zombisA[i].edx = 0;
        tss_zombisA[i].ebx = 0;
        tss_zombisA[i].esp = (int) 0x8001000;
        tss_zombisA[i].ebp = (int) 0x8001000;
        tss_zombisA[i].esi = 0;
        tss_zombisA[i].edi = 0;

        tss_zombisA[i].es = (unsigned short) (11<<3) | 0x3;
        tss_zombisA[i].cs = (unsigned short) (10<<3) | 0x3;
        tss_zombisA[i].ss = (unsigned short) (11<<3) | 0x3;
        tss_zombisA[i].ds = (unsigned short) (11<<3) | 0x3;
        tss_zombisA[i].fs = (unsigned short) (11<<3) | 0x3;
        tss_zombisA[i].gs = (unsigned short) (11<<3) | 0x3; 

    }
    else
    {
        tss_zombisB[i].cr3 = (unsigned int ) mmu_inicializar_dir_zombi(1,x,y,i);                                        
        tss_zombisB[i].eip = (int) 0x8000000;
        tss_zombisB[i].eflags = (int) 0x202;    

        tss_zombisB[i].eax = 0;
        tss_zombisB[i].ecx = 0;
        tss_zombisB[i].edx = 0;
        tss_zombisB[i].ebx = 0;
        tss_zombisB[i].esp = (int) 0x8001000;
        tss_zombisB[i].ebp = (int) 0x8001000;
        tss_zombisB[i].esi = 0;
        tss_zombisB[i].edi = 0;

        tss_zombisB[i].es = (unsigned short) (11<<3) | 0x3;
        tss_zombisB[i].cs = (unsigned short) (10<<3) | 0x3;
        tss_zombisB[i].ss = (unsigned short) (11<<3) | 0x3;
        tss_zombisB[i].ds = (unsigned short) (11<<3) | 0x3;
        tss_zombisB[i].fs = (unsigned short) (11<<3) | 0x3;
        tss_zombisB[i].gs = (unsigned short) (11<<3) | 0x3; 
    }
  
}

