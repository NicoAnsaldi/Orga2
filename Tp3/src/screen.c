/*  por compatibilidad se omiten tildes 
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del scheduler
*/

#define MEM_VIDEO_CANT_BYTES  (2 * VIDEO_FILS * VIDEO_COLS)

#include "screen.h"
#include "i386.h"
#include "sched.h"
#include "tss.h"
#include "game.h"

int tipoZombieA = 1; // 0, 1 , 2
int tipoZombieB = 1;

int posicionA = 44;
int posicionB = 44;

int cr0_e;
int cr2_e;
int cr4_e;

int stack1;
int stack2;
int stack3;
int stack4;
int stack5;

void print_reloj(){

    if (jugador_actual == 0){

        if (s.tareasA[s.indice_zombie_actualA].activo){
            if (s.tareasA[s.indice_zombie_actualA].reloj == 4) s.tareasA[s.indice_zombie_actualA].reloj = 0; 
            if (s.tareasA[s.indice_zombie_actualA].reloj == 0) print_mapa("|", (s.indice_zombie_actualA*2) + 5, 48, C_BG_BLACK + C_FG_WHITE);
            if (s.tareasA[s.indice_zombie_actualA].reloj == 1) print_mapa("/", (s.indice_zombie_actualA*2) + 5, 48, C_BG_BLACK + C_FG_WHITE);
            if (s.tareasA[s.indice_zombie_actualA].reloj == 2) print_mapa("-", (s.indice_zombie_actualA*2) + 5, 48, C_BG_BLACK + C_FG_WHITE);
            if (s.tareasA[s.indice_zombie_actualA].reloj == 3) print_mapa("\\", (s.indice_zombie_actualA*2) + 5, 48, C_BG_BLACK + C_FG_WHITE);
            s.tareasA[s.indice_zombie_actualA].reloj++;
        }  
    }
    else
    {
        if (s.tareasB[s.indice_zombie_actualB].activo){
            if (s.tareasB[s.indice_zombie_actualB].reloj == 4) s.tareasB[s.indice_zombie_actualB].reloj = 0; 
            if (s.tareasB[s.indice_zombie_actualB].reloj == 0) print_mapa("|", (s.indice_zombie_actualB*2) + 59, 48, C_BG_BLACK + C_FG_WHITE);
            if (s.tareasB[s.indice_zombie_actualB].reloj == 1) print_mapa("/", (s.indice_zombie_actualB*2) + 59, 48, C_BG_BLACK + C_FG_WHITE);
            if (s.tareasB[s.indice_zombie_actualB].reloj == 2) print_mapa("-", (s.indice_zombie_actualB*2) + 59, 48, C_BG_BLACK + C_FG_WHITE);
            if (s.tareasB[s.indice_zombie_actualB].reloj == 3) print_mapa("\\", (s.indice_zombie_actualB*2) + 59, 48, C_BG_BLACK + C_FG_WHITE);
            s.tareasB[s.indice_zombie_actualB].reloj++;
        }  
    }
}

void cambiar_tipoA(unsigned int tipo ){
    tipoZombieA = tipoZombieA + tipo;
    if (tipoZombieA == 3) tipoZombieA = 0;
    if (tipoZombieA == -1) tipoZombieA = 2;
    print_Zombie(tipoZombieA,0,posicionA,C_BG_RED + C_FG_WHITE);
}

void cambiar_tipoB(unsigned int tipo ){
    tipoZombieB = tipoZombieB + tipo;
    if (tipoZombieB == 3) tipoZombieB = 0;
    if (tipoZombieB == -1) tipoZombieB = 2;
    print_Zombie(tipoZombieB,79,posicionB,C_BG_BLUE + C_FG_WHITE);

}

void print_Zombie(unsigned int n, unsigned int x,  unsigned int y, unsigned short attr){
    if (n == 0){ 
        print_mapa("G",x,y,attr);
    }
    if (n == 1) {
        print_mapa("M",x,y,attr);   
    }
    if (n == 2){
        print_mapa("C",x,y,attr);
    }
}


void cambiar_posicionA(unsigned int pos ){
    print_mapa(" ",0,posicionA,C_BG_RED + C_FG_WHITE);
    posicionA = posicionA + pos;
    if (posicionA == 45) posicionA = 1;
    if (posicionA == 0) posicionA = 44;
    print_Zombie(tipoZombieA,0,posicionA,C_BG_RED + C_FG_WHITE);    
}

void cambiar_posicionB(unsigned int pos ){
    print_mapa(" ",79,posicionB,C_BG_BLUE + C_FG_WHITE);
    posicionB = posicionB + pos;
    if (posicionB == 45) posicionB = 1;
    if (posicionB == 0) posicionB = 44;
    print_Zombie(tipoZombieB,79,posicionB,C_BG_BLUE + C_FG_WHITE);
}

void print_mapa(const char * text, unsigned int x, unsigned int y, unsigned short attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;
    int i;
    for (i = 0; text[i] != 0; i++) {
        p[y][x].c = (unsigned char) text[i];
        p[y][x].a = (unsigned char) attr;
        x++;
        if (x == VIDEO_COLS) {
            x = 0;
            y++;
        }
    }
    
}

void print_hex(unsigned int numero, int size, unsigned int x, unsigned int y, unsigned short attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO; // magia
    int i;
    char hexa[8];
    char letras[16] = "0123456789ABCDEF";
    hexa[0] = letras[ ( numero & 0x0000000F ) >> 0  ];
    hexa[1] = letras[ ( numero & 0x000000F0 ) >> 4  ];
    hexa[2] = letras[ ( numero & 0x00000F00 ) >> 8  ];
    hexa[3] = letras[ ( numero & 0x0000F000 ) >> 12 ];
    hexa[4] = letras[ ( numero & 0x000F0000 ) >> 16 ];
    hexa[5] = letras[ ( numero & 0x00F00000 ) >> 20 ];
    hexa[6] = letras[ ( numero & 0x0F000000 ) >> 24 ];
    hexa[7] = letras[ ( numero & 0xF0000000 ) >> 28 ];
    for(i = 0; i < size; i++) {
        p[y][x + size - i - 1].c = hexa[i];
        p[y][x + size - i - 1].a = attr;
    }
}

void print_int(unsigned int n, unsigned int x, unsigned int y, unsigned short attr) {
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO; // magia
    if( n > 9 ) {
      int a = n / 10;
      n -= 10 * a;
      print_int(a,x-1,y,attr);
    }
    p[y][x].c = '0'+n;
    p[y][x].a = attr;
}


void print_teclado(const char * text, unsigned int x, unsigned int y, unsigned short attr) {

        if (*text == 0x2a) {
            print_mapa("LShift",74,0,C_BG_RED + C_FG_WHITE);
        }
        if (*text == 0x36) {
            print_mapa("RShift",74,0,C_BG_BLUE + C_FG_WHITE);
        }
        if (*text == 0x15) {
            print_mapa("     ",74,0,C_BG_BLACK + C_FG_WHITE);
            print_mapa("y",79,0,C_BG_BLACK + C_FG_WHITE);
        }
        if (*text == 0x11) {
            print_mapa("     ",74,0,C_BG_BLACK + C_FG_WHITE);
            print_mapa("w",79,0,C_BG_RED + C_FG_WHITE);
        }
        if (*text == 0x1f) {
            print_mapa("     ",74,0,C_BG_BLACK + C_FG_WHITE);
            print_mapa("s",79,0,C_BG_RED + C_FG_WHITE);
        }
        if (*text == 0x1e) {
            print_mapa("     ",74,0,C_BG_BLACK + C_FG_WHITE);
            print_mapa("a",79,0,C_BG_RED + C_FG_WHITE);
        }
        if (*text == 0x20) {
            print_mapa("     ",74,0,C_BG_BLACK + C_FG_WHITE);
            print_mapa("d",79,0,C_BG_RED + C_FG_WHITE);
        }
        if (*text == 0x17) {
            print_mapa("     ",74,0,C_BG_BLACK + C_FG_WHITE);
            print_mapa("i",79,0,C_BG_BLUE + C_FG_WHITE);
        }
        if (*text == 0x25) {
            print_mapa("     ",74,0,C_BG_BLACK + C_FG_WHITE);
            print_mapa("k",79,0,C_BG_BLUE + C_FG_WHITE);
        }
        if (*text == 0x26) {
            print_mapa("     ",74,0,C_BG_BLACK + C_FG_WHITE);
            print_mapa("l",79,0,C_BG_BLUE + C_FG_WHITE);
        }
        if (*text == 0x24) {
            print_mapa("     ",74,0,C_BG_BLACK + C_FG_WHITE);
            print_mapa("j",79,0,C_BG_BLUE + C_FG_WHITE);
        }
}


void print_nombre(){
    print_mapa("MINI OSO POLAR ",0,0,C_FG_WHITE);
}


void iniciar_pantalla_mapa(){
    int i ;
    int j ;
    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;

    for (i = 0 ; i < VIDEO_COLS;i++){
        for(j = 0; j < VIDEO_FILS ; j++){
                p[j][i].c=219;
                p[j][i].a=C_FG_GREEN;
        }
    }

    for (i = 0 ; i < VIDEO_COLS;i++){
        for(j = 0; j < 1 ; j++){
                p[j][i].c=0;
                p[j][i].a=0;
        }
    }

    for (i = 0 ; i < VIDEO_COLS;i++){
        for(j = 45; j < VIDEO_FILS ; j++){
                p[j][i].c=0;
                p[j][i].a=0;
        }
    }

    for (i = 0 ; i < 1;i++){
        for(j = 1; j < VIDEO_FILS -5 ; j++){
                p[j][i].c=219;
                p[j][i].a=C_FG_RED;
        }
    }


    for (i = VIDEO_COLS-1 ; i < VIDEO_COLS;i++){
        for(j = 1; j < VIDEO_FILS -5  ; j++){
                p[j][i].c=219;
                p[j][i].a=C_FG_BLUE;
        }
    }


        for (i = 34 ; i < 40;i++){
        for(j = 45; j < VIDEO_FILS  ; j++){
                p[j][i].c=219;
                p[j][i].a=C_FG_RED;
        }
    }

    for (i = 40 ; i < 46;i++){
        for(j = 45; j < VIDEO_FILS  ; j++){
                p[j][i].c=219;
                p[j][i].a=C_FG_BLUE;
        }
    }

    print_mapa("1",5,46,C_BG_BLACK + C_FG_WHITE);
    print_mapa("2",7,46,C_BG_BLACK + C_FG_WHITE);
    print_mapa("3",9,46,C_BG_BLACK + C_FG_WHITE);
    print_mapa("4",11,46,C_BG_BLACK + C_FG_WHITE);
    print_mapa("5",13,46,C_BG_BLACK + C_FG_WHITE);
    print_mapa("6",15,46,C_BG_BLACK + C_FG_WHITE);
    print_mapa("7",17,46,C_BG_BLACK + C_FG_WHITE);
    print_mapa("8",19,46,C_BG_BLACK + C_FG_WHITE);

    print_mapa("1",59,46,C_BG_BLACK + C_FG_WHITE);
    print_mapa("2",61,46,C_BG_BLACK + C_FG_WHITE);
    print_mapa("3",63,46,C_BG_BLACK + C_FG_WHITE);
    print_mapa("4",65,46,C_BG_BLACK + C_FG_WHITE);
    print_mapa("5",67,46,C_BG_BLACK + C_FG_WHITE);
    print_mapa("6",69,46,C_BG_BLACK + C_FG_WHITE);
    print_mapa("7",71,46,C_BG_BLACK + C_FG_WHITE);
    print_mapa("8",73,46,C_BG_BLACK + C_FG_WHITE);

    print_Zombie(tipoZombieA,0,posicionA,C_BG_RED + C_FG_WHITE);
    print_Zombie(tipoZombieB,79,posicionB,C_BG_BLUE + C_FG_WHITE);
    print_puntos(0);
    print_puntos(1);
    print_mapa("Modo Debug Activado:",29,0,C_BG_BLACK + C_FG_WHITE);
    print_mapa("NO",49,0,C_BG_BLACK + C_FG_WHITE);
}

void donde_estoy(unsigned int jugador, unsigned int tarea){
    if (jugador == 0){
        sched_entry i = s.tareasA[tarea];
        print_Zombie(i.tipoZombie, i.posx, i.posy, C_BG_RED + C_FG_WHITE);
    }
    else
    {
        sched_entry i = s.tareasB[tarea];
        print_Zombie(i.tipoZombie, i.posx, i.posy, C_BG_BLUE + C_FG_WHITE);
    }
}

void donde_estaba(unsigned int jugador, unsigned int tarea){
    if (jugador == 0){
        print_mapa("X", s.tareasA[tarea].posx, s.tareasA[tarea].posy, C_BG_GREEN + C_FG_LIGHT_GREY);
    }
    else
    {
        print_mapa("X", s.tareasB[tarea].posx, s.tareasB[tarea].posy, C_BG_GREEN + C_FG_LIGHT_GREY);
    }
}

void print_puntos()
{
    print_int(puntosA, 36, 47, C_BG_RED + C_FG_WHITE);
    print_int(puntosB, 42, 47, C_BG_BLUE + C_FG_WHITE);
}

void iniciar_estado(){
    int i ;
    int j ;

    ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;

    for (i = 24 ; i < 55;i++){
        for(j = 6; j < 43 ; j++){
                p[j][i].c=0;
                p[j][i].a=0;
        }
    }

    for (i = 25 ; i < 54;i++){
        for(j = 7; j < 42 ; j++){
                p[j][i].c=219;
                p[j][i].a=C_FG_LIGHT_GREY;
        }
    }

    for (i = 25 ; i < 54;i++){
        for(j = 7; j < 8 ; j++){
                p[j][i].c=219;
                p[j][i].a=C_FG_BLUE;
        }
    }

    print_mapa("Zombi", 25,7, C_BG_BLUE + C_FG_WHITE);

    print_mapa("eax",26,10, C_BG_LIGHT_GREY + C_FG_BLACK);
    print_mapa("ebx", 26,12, C_BG_LIGHT_GREY + C_FG_BLACK);
    print_mapa("ecx", 26,14, C_BG_LIGHT_GREY + C_FG_BLACK);
    print_mapa("edx", 26,16, C_BG_LIGHT_GREY + C_FG_BLACK);
    print_mapa("esi", 26,18, C_BG_LIGHT_GREY + C_FG_BLACK);
    print_mapa("edi", 26,20, C_BG_LIGHT_GREY + C_FG_BLACK);
    print_mapa("ebp", 26,22, C_BG_LIGHT_GREY + C_FG_BLACK);
    print_mapa("esp", 26,24, C_BG_LIGHT_GREY + C_FG_BLACK);
    print_mapa("eip", 26,26, C_BG_LIGHT_GREY + C_FG_BLACK);
    print_mapa("cr0", 40,10, C_BG_LIGHT_GREY + C_FG_BLACK);
    print_mapa("cr2", 40,12, C_BG_LIGHT_GREY + C_FG_BLACK);
    print_mapa("cr3", 40,14, C_BG_LIGHT_GREY + C_FG_BLACK);
    print_mapa("cr4", 40,16, C_BG_LIGHT_GREY + C_FG_BLACK);
    print_mapa("cs", 27,28, C_BG_LIGHT_GREY + C_FG_BLACK);
    print_mapa("ds", 27,30, C_BG_LIGHT_GREY + C_FG_BLACK);
    print_mapa("es", 27,32, C_BG_LIGHT_GREY + C_FG_BLACK);
    print_mapa("fs", 27,34, C_BG_LIGHT_GREY + C_FG_BLACK);
    print_mapa("gs", 27,36, C_BG_LIGHT_GREY + C_FG_BLACK);
    print_mapa("ss", 27,38, C_BG_LIGHT_GREY + C_FG_BLACK);
    print_mapa("eflags", 27,40, C_BG_LIGHT_GREY + C_FG_BLACK);
    print_mapa("stack", 40,28, C_BG_LIGHT_GREY + C_FG_BLACK);

}


void memcopy(int dst, int src, int size) {
    
    char* src_char = (char*) src;
    char* dst_char = (char*) dst;

    int i;
    for(i = 0; i < size ; i++) {
        dst_char[i] = src_char[i];
    }

}


void print_tss(unsigned int errorcode, unsigned int eip, unsigned int cs, unsigned int eflags, unsigned int esp, unsigned int ss ) {
    
    
    int j = jugador_actual;
    int A = s.indice_zombie_actualA;
    int B = s.indice_zombie_actualB;
    if (j == 0){
        sched_sacar(j, A);
    }
    else
    {
        sched_sacar(j, B);
    }



    if (modoDebugFueActivado == 1){
        modoDebug = 1;
        memcopy(ADDR_BUFFER_VIDEO_MAPA, VIDEO, MEM_VIDEO_CANT_BYTES);
        iniciar_estado();

        ca (*p)[VIDEO_COLS] = (ca (*)[VIDEO_COLS]) VIDEO;

        if (j == 0){
            int i;
            for (i = 25 ; i < 54;i++){
                for(j = 7; j < 8 ; j++){
                    p[j][i].c=219;
                    p[j][i].a=C_FG_RED;
                }
            }

            print_mapa("Zombi N", 25,7, C_BG_RED + C_FG_WHITE);

            print_int(A+1, 34,7, C_BG_RED + C_FG_WHITE);

            if (s.tareasA[A].tipoZombie == 0){ 
                print_mapa("Guerrero", 36,7, C_BG_RED + C_FG_WHITE);
            }
            if (s.tareasA[A].tipoZombie == 1) {
                print_mapa("Mago",36,7, C_BG_RED + C_FG_WHITE);   
            }
            if (s.tareasA[A].tipoZombie == 2){
                print_mapa("Clerigo",36,7, C_BG_RED + C_FG_WHITE);
            }

            print_hex(tss_zombisA[A].eax,8,30,10, C_BG_LIGHT_GREY + C_FG_WHITE);
            print_hex(tss_zombisA[A].ebx,8, 30,12, C_BG_LIGHT_GREY + C_FG_WHITE);
            print_hex(tss_zombisA[A].ecx,8, 30,14, C_BG_LIGHT_GREY + C_FG_WHITE);
            print_hex(tss_zombisA[A].edx,8, 30,16, C_BG_LIGHT_GREY + C_FG_WHITE);
            print_hex(tss_zombisA[A].esi,8, 30,18, C_BG_LIGHT_GREY + C_FG_WHITE);
            print_hex(tss_zombisA[A].edi,8, 30,20, C_BG_LIGHT_GREY + C_FG_WHITE);
            print_hex(tss_zombisA[A].ebp,8, 30,22, C_BG_LIGHT_GREY + C_FG_WHITE);
            print_hex(esp,8, 30,24, C_BG_LIGHT_GREY + C_FG_WHITE);
            print_hex(eip,8, 30,26, C_BG_LIGHT_GREY + C_FG_WHITE);
            print_hex(cr0_e,8, 44,10, C_BG_LIGHT_GREY + C_FG_WHITE);
            print_hex(cr2_e,8, 44,12, C_BG_LIGHT_GREY + C_FG_WHITE);
            print_hex(tss_zombisA[A].cr3,8, 44,14, C_BG_LIGHT_GREY + C_FG_WHITE);
            print_hex(cr4_e,8, 44,16, C_BG_LIGHT_GREY + C_FG_WHITE);
            print_hex(cs,4, 30,28, C_BG_LIGHT_GREY + C_FG_WHITE);
            print_hex(tss_zombisA[A].ds,4, 30,30, C_BG_LIGHT_GREY + C_FG_WHITE);
            print_hex(tss_zombisA[A].es,4, 30,32, C_BG_LIGHT_GREY + C_FG_WHITE);
            print_hex(tss_zombisA[A].fs,4, 30,34, C_BG_LIGHT_GREY + C_FG_WHITE);
            print_hex(tss_zombisA[A].gs,4, 30,36, C_BG_LIGHT_GREY + C_FG_WHITE);
            print_hex(ss,4, 30,38, C_BG_LIGHT_GREY + C_FG_WHITE);
            print_hex(eflags,8, 35,40, C_BG_LIGHT_GREY + C_FG_WHITE);

            print_hex(stack1,8,40,32, C_BG_LIGHT_GREY + C_FG_WHITE);
            print_hex(stack2,8, 40,33, C_BG_LIGHT_GREY + C_FG_WHITE);
            print_hex(stack3,8, 40,34, C_BG_LIGHT_GREY + C_FG_WHITE);
            print_hex(stack4,8, 40,35, C_BG_LIGHT_GREY + C_FG_WHITE);
            print_hex(stack5,8, 40,36, C_BG_LIGHT_GREY + C_FG_WHITE);
            //breakpoint();
        }
        else
        {
            int i;  
            for (i = 25 ; i < 54;i++){
                for(j = 7; j < 8 ; j++){
                    p[j][i].c=219;
                    p[j][i].a=C_FG_BLUE;
                }
            }

            print_mapa("Zombi N", 25,7, C_BG_BLUE + C_FG_WHITE);

            print_int(B+1, 34,7, C_BG_BLUE + C_FG_WHITE);

            if (s.tareasB[B].tipoZombie == 0){ 
                print_mapa("Guerrero", 36,7, C_BG_BLUE + C_FG_WHITE);
            }
            if (s.tareasB[B].tipoZombie == 1) {
                print_mapa("Mago",36,7, C_BG_BLUE + C_FG_WHITE);   
            }
            if (s.tareasB[B].tipoZombie == 2){
                print_mapa("Clerigo",36,7, C_BG_BLUE + C_FG_WHITE);
            }

            print_hex(tss_zombisB[B].eax,8,30,10, C_BG_LIGHT_GREY + C_FG_WHITE);
            print_hex(tss_zombisB[B].ebx,8, 30,12, C_BG_LIGHT_GREY + C_FG_WHITE);
            print_hex(tss_zombisB[B].ecx,8, 30,14, C_BG_LIGHT_GREY + C_FG_WHITE);
            print_hex(tss_zombisB[B].edx,8, 30,16, C_BG_LIGHT_GREY + C_FG_WHITE);
            print_hex(tss_zombisB[B].esi,8, 30,18, C_BG_LIGHT_GREY + C_FG_WHITE);
            print_hex(tss_zombisB[B].edi,8, 30,20, C_BG_LIGHT_GREY + C_FG_WHITE);
            print_hex(tss_zombisB[B].ebp,8, 30,22, C_BG_LIGHT_GREY + C_FG_WHITE);
            print_hex(esp,8, 30,24, C_BG_LIGHT_GREY + C_FG_WHITE);
            print_hex(eip,8, 30,26, C_BG_LIGHT_GREY + C_FG_WHITE);
            print_hex(cr0_e,8, 44,10, C_BG_LIGHT_GREY + C_FG_WHITE);
            print_hex(cr2_e,8, 44,12, C_BG_LIGHT_GREY + C_FG_WHITE);
            print_hex(tss_zombisB[B].cr3,8, 44,14, C_BG_LIGHT_GREY + C_FG_WHITE);
            print_hex(cr4_e,8, 44,16, C_BG_LIGHT_GREY + C_FG_WHITE);
            print_hex(cs,4, 30,28, C_BG_LIGHT_GREY + C_FG_WHITE);
            print_hex(tss_zombisB[B].ds,4, 30,30, C_BG_LIGHT_GREY + C_FG_WHITE);
            print_hex(tss_zombisB[B].es,4, 30,32, C_BG_LIGHT_GREY + C_FG_WHITE);
            print_hex(tss_zombisB[B].fs,4, 30,34, C_BG_LIGHT_GREY + C_FG_WHITE);
            print_hex(tss_zombisB[B].gs,4, 30,36, C_BG_LIGHT_GREY + C_FG_WHITE);
            print_hex(ss,4, 30,38, C_BG_LIGHT_GREY + C_FG_WHITE);
            print_hex(eflags,8, 35,40, C_BG_LIGHT_GREY + C_FG_WHITE);

            print_hex(stack1,8,40,32, C_BG_LIGHT_GREY + C_FG_WHITE);
            print_hex(stack2,8, 40,33, C_BG_LIGHT_GREY + C_FG_WHITE);
            print_hex(stack3,8, 40,34, C_BG_LIGHT_GREY + C_FG_WHITE);
            print_hex(stack4,8, 40,35, C_BG_LIGHT_GREY + C_FG_WHITE);
            print_hex(stack5,8, 40,36, C_BG_LIGHT_GREY + C_FG_WHITE);  
        }
    }
}

void mostrar_mapa(){
    memcopy( VIDEO, ADDR_BUFFER_VIDEO_MAPA, MEM_VIDEO_CANT_BYTES);
}


void estoy_en_estado(){
    if (modoDebugFueActivado == 1){
        print_mapa("SI",49,0,C_BG_BLACK + C_FG_WHITE);
    }
}













































/*
void print_isr(unsigned int n, unsigned short task, unsigned int fila){


       if (n == 0) {
            char msj_error2[20]  = " Divide by Zero    \0";
            print_mapa(msj_error2,50,(15+task),C_BG_RED + C_FG_WHITE);
            print_mapa(msj_error2,50,1,C_BG_CYAN + C_FG_WHITE); 
        }
        if (n == 1) {
            char msj_error[20] = " Debug Exception   \0";
            print_mapa(msj_error,50,(15+task),C_BG_RED + C_FG_WHITE);
            print_mapa(msj_error,50,1,C_BG_CYAN + C_FG_WHITE);
        }
        if (n == 2) {
            char msj_error[20] = " NMI Interrupt     \0";
            print_mapa(msj_error,50,(15+task),C_BG_RED + C_FG_WHITE);
            print_mapa(msj_error,50,1,C_BG_CYAN + C_FG_WHITE);
        }          
        if (n == 3) {
            char msj_error[20] = " Breakpoint        \0";
            print_mapa(msj_error,50,(15+task),C_BG_RED + C_FG_WHITE);
            print_mapa(msj_error,50,1,C_BG_CYAN + C_FG_WHITE);
        }
        if (n == 4) {
            char msj_error[20] = " Overflow          \0";
            print_mapa(msj_error,50,(15+task),C_BG_RED + C_FG_WHITE);
            print_mapa(msj_error,50,1,C_BG_CYAN + C_FG_WHITE);
        }
        if (n == 5) {
            char msj_error[20] = " BOUND Range Execee\0";
            print_mapa(msj_error,50,(15+task),C_BG_RED + C_FG_WHITE);
            print_mapa(msj_error,50,1,C_BG_CYAN + C_FG_WHITE);
        }
        if (n == 6) {
            char msj_error[20] = " Invalid Opcode    \0"; //undefined opcode
            print_mapa(msj_error,50,(15+task),C_BG_RED + C_FG_WHITE);
            print_mapa(msj_error,50,1,C_BG_CYAN + C_FG_WHITE);
        }
        if (n == 7) {
            char msj_error[20] = " Device Not Availab\0"; //(No Math Coprocessor)
            print_mapa(msj_error,50,(15+task),C_BG_RED + C_FG_WHITE);
            print_mapa(msj_error,50,1,C_BG_CYAN + C_FG_WHITE);
        }
        if (n == 8) {
            char msj_error[20] = " Double Fault      \0";
            print_mapa(msj_error,50,(15+task),C_BG_RED + C_FG_WHITE);
            print_mapa(msj_error,50,1,C_BG_CYAN + C_FG_WHITE);
        }
        if (n == 9) {
            char msj_error[20] = " CoprocessorSegOver\0"; //Coprocessor Segment Overrun (reserved) 
            print_mapa(msj_error,50,(15+task),C_BG_RED + C_FG_WHITE);
            print_mapa(msj_error,50,1,C_BG_CYAN + C_FG_WHITE);
        }
        if (n == 10) {
            char msj_error[20] = " Invalid TSS       \0";
            print_mapa(msj_error,50,(15+task),C_BG_RED + C_FG_WHITE);
            print_mapa(msj_error,50,1,C_BG_CYAN + C_FG_WHITE);
        }
        if (n == 11) {
            char msj_error[20] = " Segment Not Presen\0";
            print_mapa(msj_error,50,(15+task),C_BG_RED + C_FG_WHITE);
            print_mapa(msj_error,50,1,C_BG_CYAN + C_FG_WHITE);
        }
        if (n == 12) {
            char msj_error[20] = " Stack-Segment Faul\0";
            print_mapa(msj_error,50,(15+task),C_BG_RED + C_FG_WHITE);
            print_mapa(msj_error,50,1,C_BG_CYAN + C_FG_WHITE);
        }
        if (n == 13) {
            char msj_error[20] = " General Protection\0";
            print_mapa(msj_error,50,(15+task),C_BG_RED + C_FG_WHITE);
            print_mapa(msj_error,50,1,C_BG_CYAN + C_FG_WHITE);
        }
        if (n == 14) {
            char msj_error[20] = " Page Fault        \0";
            print_mapa(msj_error,50,(15+task),C_BG_RED + C_FG_WHITE);
            print_mapa(msj_error,50,1,C_BG_CYAN + C_FG_WHITE);
        }
        if (n == 15) {
            char msj_error[20] = " intel reserved    \0";
            print_mapa(msj_error,50,(15+task),C_BG_RED + C_FG_WHITE);
            print_mapa(msj_error,50,1,C_BG_CYAN + C_FG_WHITE);
        }
        if (n == 16) {
            char msj_error[20] = "Floating-Point Erro\0"; //x87 FPU Floating-Point Error (Math Fault)
            print_mapa(msj_error,50,(15+task),C_BG_RED + C_FG_WHITE);
            print_mapa(msj_error,50,1,C_BG_CYAN + C_FG_WHITE);
        }
        if (n == 17) {
            char msj_error[20] = " Alignment Check   \0";
            print_mapa(msj_error,50,(15+task),C_BG_RED + C_FG_WHITE);
            print_mapa(msj_error,50,1,C_BG_CYAN + C_FG_WHITE);
        }
        if (n == 18) {
            char msj_error[20] = " Machine Check     \0";
            print_mapa(msj_error,50,(15+task),C_BG_RED + C_FG_WHITE);
            print_mapa(msj_error,50,1,C_BG_CYAN + C_FG_WHITE);
        }
        if (n == 19) {
            char msj_error[20] = " SIMD FloatPoint Ex\0";//SIMD Floating-Point Exception
            print_mapa(msj_error,50,(15+task),C_BG_RED + C_FG_WHITE);
            print_mapa(msj_error,50,1,C_BG_CYAN + C_FG_WHITE);
        }
        if (n == 20) {
            char msj_error[20] = " Virtualization Exc\0"; //Virtualization Exception
            print_mapa(msj_error,50,(15+task),C_BG_RED + C_FG_WHITE);
            print_mapa(msj_error,50,1,C_BG_CYAN + C_FG_WHITE);
        }
        if (n == 34) {
            char msj_error[20] = " Syscall ...\0"; //Virtualization Exception
            print_mapa(msj_error,50,(15+task),C_BG_RED + C_FG_WHITE);
            print_mapa(msj_error,50,1,C_BG_CYAN + C_FG_WHITE);
        }
}*/