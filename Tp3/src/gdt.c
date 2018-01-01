/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de la tabla de descriptores globales
*/

#include "gdt.h"
#include "tss.h"
#include "screen.h"

gdt_entry gdt[GDT_COUNT] = {
    /* Descriptor nulo*/
    /* Offset = 0x00 */
    [GDT_IDX_NULL_DESC] = (gdt_entry) {
        (unsigned short)    0x0000,         /* limit[0:15]  */
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x00,           /* type         */
        (unsigned char)     0x00,           /* s            */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x00,           /* p            */
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x00,           /* db           */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },

    //codigo nivel 0
    [GDT_IDX_NULL_DESC+8] = (gdt_entry) {
        (unsigned short)    0x6eff,         /* limit[0:15]  */
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x08,           /* type         */  
        (unsigned char)     0x01,           /* s            */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x01,           /* p            */ 
        (unsigned char)     0x02,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */ 
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x01,           /* db           */
        (unsigned char)     0x01,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },

    //datos nivel 0
    [GDT_IDX_NULL_DESC+9] = (gdt_entry) {
        (unsigned short)    0x6eff,         /* limit[0:15]  */
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x02,           /* type         */  
        (unsigned char)     0x01,           /* s            */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x01,           /* p            */ 
        (unsigned char)     0x02,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */ 
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x01,           /* db           */
        (unsigned char)     0x01,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },
    //codigo nivel 3
    [GDT_IDX_NULL_DESC+10] = (gdt_entry) {
        (unsigned short)    0x6eff,         /* limit[0:15]  */
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x08,           /* type         */  
        (unsigned char)     0x01,           /* s            */
        (unsigned char)     0x03,           /* dpl          */
        (unsigned char)     0x01,           /* p            */ 
        (unsigned char)     0x02,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */ 
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x01,           /* db           */
        (unsigned char)     0x01,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },

    //datos nivel 3
    [GDT_IDX_NULL_DESC+11] = (gdt_entry) {
        (unsigned short)    0x6eff,         /* limit[0:15]  */
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x02,           /* type         */  
        (unsigned char)     0x01,           /* s            */
        (unsigned char)     0x03,           /* dpl          */
        (unsigned char)     0x01,           /* p            */ 
        (unsigned char)     0x02,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */ 
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x01,           /* db           */
        (unsigned char)     0x01,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    },
    //video
     [GDT_IDX_NULL_DESC+12] = (gdt_entry) {
        (unsigned short)    0x6eff,         /* limit[0:15]  */
        (unsigned short)    0x0000,         /* base[0:15]   */
        (unsigned char)     0x0b,           /* base[23:16]  */
        (unsigned char)     0x02,           /* type         */  
        (unsigned char)     0x01,           /* s            */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x01,           /* p            */ 
        (unsigned char)     0x02,           /* limit[16:19] */
        (unsigned char)     0x00,           /* avl          */ 
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x01,           /* db           */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
    }



};

gdt_descriptor GDT_DESC = {
    sizeof(gdt) - 1,
    (unsigned int) &gdt
};



void inicializar_gdt_tareas(){

//TAREA INICIAL
    gdt[13] = (gdt_entry) {
        (unsigned short)    0x67,         /* limit[0:15]  */
        (unsigned short)   (int) &tss_inicial,         /* base[0:15]   */
        (unsigned char)    (int) &tss_inicial>>16,           /* base[23:16]  */
        (unsigned char)     0x09,           /* type         */  //exc-only, accesed
        (unsigned char)     0x00,           /* s            */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x01,           /* p            */ 
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x01,           /* avl          */ //fafafa
        (unsigned char)     0x00,           /* l            */ 
        (unsigned char)     0x00,           /* db           */ //Esto va en cero?
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     (int) &tss_inicial>>24,           /* base[31:24]  */
    };

    //gdt[13].base_0_15 =(int) &tss_inicial;
    //gdt[13].base_23_16 =(int) &tss_inicial>>16;
    //gdt[13].base_31_24 =(int) &tss_inicial>>24;


//TAREA IDLE
     gdt[14] = (gdt_entry) {
        (unsigned short)    0x67,         /* limit[0:15]  */
        (unsigned short)   (int) &tss_idle,           /* base[0:15]   */
        (unsigned char)    (int) (&tss_idle)>>16,           /* base[23:16]  */
        (unsigned char)     0x09,           /* type         */  
        (unsigned char)     0x00,           /* s            */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x01,           /* p            */ 
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x01,           /* avl          */ //freuta
        (unsigned char)     0x00,           /* l            */
        (unsigned char)     0x00,           /* db           */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     (int )(&tss_idle)>>24,           /* base[31:24]  */
    };

    //gdt[14].base_0_15  =(int) &tss_idle;
    //gdt[14].base_23_16 =(int) (&tss_idle)>>16;
    //gdt[14].base_31_24 =(int) (&tss_idle)>>24;



    int i;
    for(i = 0 ; i<8; i++) // 8 zombiesA
    {
        //ZOmbiess
        gdt[15+i] = (gdt_entry) {
        (unsigned short)    0x67,         /* limit[0:15]  */
        (unsigned short)    0x00,           /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x09,           /* type         */  
        (unsigned char)     0x00,           /* s            */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x01,           /* p            */ 
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x01,           /* avl          */ 
        (unsigned char)     0x00,           /*              */
        (unsigned char)     0x00,           /*              */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
        };

        gdt[15+i].base_0_15 =  ((unsigned int  ) &tss_zombisA[i]) & 0x0000ffff;
        gdt[15+i].base_23_16 = ((((unsigned int )&tss_zombisA[i]) & 0x00ff0000)>>16);
        gdt[15+i].base_31_24 = ((((unsigned int )&tss_zombisA[i]) & 0xff000000)>>24);
    }    
        for(i = 0 ; i<8; i++) // 8 zombiesB
    {
        //ZOmbiess
        gdt[23+i] = (gdt_entry) {
        (unsigned short)    0x67,         /* limit[0:15]  */
        (unsigned short)    0x00,           /* base[0:15]   */
        (unsigned char)     0x00,           /* base[23:16]  */
        (unsigned char)     0x09,           /* type         */  
        (unsigned char)     0x00,           /* s            */
        (unsigned char)     0x00,           /* dpl          */
        (unsigned char)     0x01,           /* p            */ 
        (unsigned char)     0x00,           /* limit[16:19] */
        (unsigned char)     0x01,           /* avl          */ 
        (unsigned char)     0x00,           /*              */
        (unsigned char)     0x00,           /*              */
        (unsigned char)     0x00,           /* g            */
        (unsigned char)     0x00,           /* base[31:24]  */
        };

        gdt[23+i].base_0_15 =  ((unsigned int  ) &tss_zombisB[i]) & 0x0000ffff;
        gdt[23+i].base_23_16 = ((((unsigned int )&tss_zombisB[i]) & 0x00ff0000)>>16);
        gdt[23+i].base_31_24 = ((((unsigned int )&tss_zombisB[i]) & 0xff000000)>>24);
    }  
}

