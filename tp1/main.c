#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "redcaminera.h"

int main(){
	//Definiciones
	redCaminera* r = rc_crear("kukamonga");
	//ciudad* c1 = c_crear("Montebello", 12041);
	//ciudad* c2 = c_crear("North haverbrook", 1244);
	//ciudad* c3 = c_crear("Cocula", 342);
	//ruta* r1 = r_crear(c1, c2, 232);
	//ruta* r2 = r_crear(c1, c3, 233);
	//ruta* r3 = r_crear(c2,c3,236);
	//lista* l = l_crear();
	//lista* l2 = l_crear();
	//-------------------------------------------
	//Test lista
	/*
	//l_agregarAdelante(&l , c1, c_borrar);
	//l_agregarAtras(&l , c2, c_borrar);
	l_agregarOrdenado(&l ,c3,c_borrar,c_cmp);
	l_agregarOrdenado(&l ,c2,c_borrar,c_cmp);
	l_agregarOrdenado(&l ,c1,c_borrar,c_cmp);
	l_agregarOrdenado(&l2, r1,r_borrar,r_cmp);
	l_agregarOrdenado(&l2, r2,r_borrar,r_cmp);
	l_agregarOrdenado(&l2, r3,r_borrar,r_cmp);
	*/
	//--------------------------------------------
	//Test ciudad
	/*
	printf ("%d, deberia dar 0 \n", c_cmp(c1,c1));
	printf ("%d, deberia dar 1 \n", c_cmp(c1,c2));
	printf ("%d, deberia dar -1 \n", c_cmp(c1,c3));
	printf ("%d, deberia dar 1 \n", c_cmp(c3,c2));
	*/
	//---------------------------------------------
	//Test ruta
	/*
	printf ("%d, deberia dar -1 \n", r_cmp(r1,r2));
	printf ("%d, deberia dar 1 \n", r_cmp(r2,r3));
	printf ("%d, deberia dar -1 \n", r_cmp(r1,r3));
	printf ("%d, deberia dar -1 \n", r_cmp(r3,r2));
	printf ("%d, deberia dar 1 \n", r_cmp(r2,r1));
	*/
	//-----------------------------------------------
	//Test red Caminera
	
	rc_agregarCiudad(r,"North haverbrook", 1244);
	/*
	printf("%s\n", ( (ciudad*) ((nodo*) ((lista*) r->ciudades)->primero)->dato)->nombre);
	printf("%lu\n\n", ( (ciudad*) ((nodo*) ((lista*) r->ciudades)->primero)->dato)->poblacion);
	*/
	rc_agregarCiudad(r,"Montebello",12041);
	/*
	printf("%s\n", ( (ciudad*) ((nodo*) ((lista*) r->ciudades)->primero)->dato)->nombre);
	printf("%lu\n", ( (ciudad*) ((nodo*) ((lista*) r->ciudades)->primero)->dato)->poblacion);
	printf("%s\n", ( (ciudad*) ((nodo*) ((lista*) r->ciudades)->primero->siguiente)->dato)->nombre);
	printf("%lu\n\n", ( (ciudad*) ((nodo*) ((lista*) r->ciudades)->primero->siguiente)->dato)->poblacion);
	*/
	rc_agregarCiudad(r, "Cocula", 342);
	/*
	printf("%s\n", ( (ciudad*) ((nodo*) ((lista*) r->ciudades)->primero)->dato)->nombre);
	printf("%lu\n", ( (ciudad*) ((nodo*) ((lista*) r->ciudades)->primero)->dato)->poblacion);
	printf("%s\n", ( (ciudad*) ((nodo*) ((lista*) r->ciudades)->primero->siguiente)->dato)->nombre);
	printf("%lu\n", ( (ciudad*) ((nodo*) ((lista*) r->ciudades)->primero->siguiente)->dato)->poblacion);
	printf("%s\n", ( (ciudad*) ((nodo*) ((lista*) r->ciudades)->primero->siguiente->siguiente)->dato)->nombre);
	printf("%lu\n", ( (ciudad*) ((nodo*) ((lista*) r->ciudades)->primero->siguiente->siguiente)->dato)->poblacion);
	
	ciudad* ce = obtenerCiudad(r, "Cocula");
	printf ("%s\n", ce->nombre);
	printf ("%lu\n", ce->poblacion);
	ce = obtenerCiudad(r, "Montebello");
	printf ("%s\n", ce->nombre);
	printf ("%lu\n", ce->poblacion);
	*/
	
	rc_agregarRuta(r, "Cocula", "Montebello", 233);
	/*
	printf("%s\n", ((ciudad*) ( (ruta*) ((nodo*) ((lista*) r->rutas)->primero)->dato)->ciudadA)	->nombre);
	printf("%s\n", ((ciudad*) ((ruta*) ((nodo*) ((lista*) r->rutas)->primero)->dato)->ciudadB)->nombre);
	printf("%f\n\n", ( (ruta*) ((nodo*) ((lista*) r->rutas)->primero)->dato)->distancia);
	*/
	rc_agregarRuta(r, "Montebello", "North haverbrook", 444);
	/*	
	printf("%s\n", ((ciudad*) ( (ruta*) ((nodo*) ((lista*) r->rutas)->primero)->dato)->ciudadA)	->nombre);
	printf("%s\n", ((ciudad*) ((ruta*) ((nodo*) ((lista*) r->rutas)->primero)->dato)->ciudadB)->nombre);
	printf("%f\n", ( (ruta*) ((nodo*) ((lista*) r->rutas)->primero)->dato)->distancia);
	printf("%s\n", ((ciudad*) ( (ruta*) ((nodo*) ((lista*) r->rutas)->primero->siguiente)->dato)->ciudadA)	->nombre);
	printf("%s\n", ((ciudad*) ((ruta*) ((nodo*) ((lista*) r->rutas)->primero->siguiente)->dato)->ciudadB)->nombre);
	printf("%f\n\n", ( (ruta*) ((nodo*) ((lista*) r->rutas)->primero->siguiente)->dato)->distancia);
	*/
	rc_agregarRuta(r, "Montebello", "Cocula", 233);
	rc_agregarRuta(r, "Cocula", "Montebello", 233);

	rc_agregarRuta(r, "Cocula", "North haverbrook", 444);
	/*
	printf("%s\n", ((ciudad*) ( (ruta*) ((nodo*) ((lista*) r->rutas)->primero)->dato)->ciudadA)	->nombre);
	printf("%s\n", ((ciudad*) ((ruta*) ((nodo*) ((lista*) r->rutas)->primero)->dato)->ciudadB)->nombre);
	printf("%f\n", ( (ruta*) ((nodo*) ((lista*) r->rutas)->primero)->dato)->distancia);
	printf("%s\n", ((ciudad*) ( (ruta*) ((nodo*) ((lista*) r->rutas)->primero->siguiente)->dato)->ciudadA)	->nombre);
	printf("%s\n", ((ciudad*) ((ruta*) ((nodo*) ((lista*) r->rutas)->primero->siguiente)->dato)->ciudadB)->nombre);
	printf("%f\n", ( (ruta*) ((nodo*) ((lista*) r->rutas)->primero->siguiente)->dato)->distancia);
	printf("%s\n", ((ciudad*) ( (ruta*) ((nodo*) ((lista*) r->rutas)->primero->siguiente->siguiente)->dato)->ciudadA)	->nombre);
	printf("%s\n", ((ciudad*) ((ruta*) ((nodo*) ((lista*) r->rutas)->primero->siguiente->siguiente)->dato)->ciudadB)->nombre);
	printf("%f\n\n", ( (ruta*) ((nodo*) ((lista*) r->rutas)->primero->siguiente->siguiente)->dato)->distancia);
	
	
	ruta* re = obtenerRuta(r, "Cocula", "Montebello");
	printf ("%s\n", (((ciudad*) re->ciudadA))->nombre);
	
	ciudad* ce2 = ciudadMasPoblada(r);
	printf("%s\n", ce2->nombre);
	printf("%lu\n", ce2->poblacion);
	
	printf("%u\n",cantidadDeCaminos(r,"Cocula"));

	printf("%lu\n", totalDePoblacion(r));

	printf("%f\n", totalDeDistancia(r));
	*/
	/*
	ciudad* ce3 = ciudadMasComunicada(r);
	printf("%s\n", ce3->nombre);
	
	ruta* re2 = rutaMasLarga(r);
	printf ("%s\n", (((ciudad*) re2->ciudadA))->nombre);
	printf ("%f\n", re2->distancia);
	*/
	/*
	ciudad* prueba = c_crear("s",0);
	ciudad* prueba2 = c_crear("s",0);
	ciudadesMasLejanas(r, &prueba, &prueba2);
	printf("%s\n", prueba->nombre);
	printf("%lu\n", prueba->poblacion);
	printf("%s\n", prueba2->nombre);
	printf("%lu\n", prueba2	->poblacion);
	*/
	//--------------------------------------------------
	//Borro todo
	//r_borrar(r3);
	//c_borrar(c1);
	//c_borrar(c2);
	//c_borrar(c3);
	//r_borrar(r1);
	//r_borrar(r2);
	//c_borrar(ce3);
	//r_borrar(re2);
	//c_borrar(prueba);
	//c_borrar(prueba2);
	//l_borrarTodo(l);
	//l_borrarTodo(l2);
	rc_borrarTodo(r);
	return 0;    
}