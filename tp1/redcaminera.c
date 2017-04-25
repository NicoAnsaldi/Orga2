#include "redcaminera.h"

void rc_imprimirTodo(redCaminera* rc, FILE * File) {
	fprintf(File, "%s\n" , "Nombre:" );
	fprintf(File, "%s\n" , rc->nombre);
	fprintf(File, "%s\n" , "Ciudades:" );

	nodo* iter = ((lista*) rc->ciudades)->primero;
	int i = ((lista*) rc->ciudades)->longitud;

	while ( i != 0){
		fprintf(File, "[%s,%ld]\n", ((ciudad*) ((nodo*) iter)->dato)->nombre, ((ciudad*) ((nodo*) iter)->dato)->poblacion );
		iter = iter->siguiente;
		i--;
	}
	fprintf(File, "%s\n" , "Rutas:" );

	iter = ((lista*) rc->rutas)->primero;
	i = ((lista*) rc->rutas)->longitud;

	while ( i != 0){
		fprintf(File, "[%s,%s,%.1f]\n", ((ruta*) ((nodo*) iter)->dato)->ciudadA->nombre, ((ruta*) ((nodo*) iter)->dato)->ciudadB->nombre,((ruta*) ((nodo*) iter)->dato)->distancia );
		iter = iter->siguiente;
		i--;
	}
}

redCaminera* rc_combinarRedes(char* nombre, redCaminera* rc1, redCaminera* rc2) {
	redCaminera* r = rc_crear(nombre);

	lista* ciudadesRC1 = rc1->ciudades;
	lista* ciudadesRC2 = rc2->ciudades;
	lista* rutasRC1 = rc1->rutas;
	lista* rutasRC2 = rc2->rutas;

	nodo* iterRC1 = ((lista*) rc1->ciudades)->primero;
	nodo* iterRC2 = ((lista*) rc2->ciudades)->primero;
	uint32_t i = 0;
	while(i< ((lista*) rc1->ciudades)->longitud){
		i++;
		rc_agregarCiudad(r, ((ciudad*)iterRC1->dato)->nombre, ((ciudad*)iterRC1->dato)->poblacion);
		iterRC1 = iterRC1->siguiente;
	}
	uint32_t j = 0;
	while(j<((lista*) rc2->ciudades)->longitud){
		i = 0;
		iterRC1 = ((lista*) rc1->ciudades)->primero;
		while((i< ((lista*) rc1->ciudades)->longitud) && !(c_cmp(((ciudad*)iterRC1->dato), ((ciudad*)iterRC2->dato))==0)){
			i++;
			iterRC1 = iterRC1->siguiente;
		}
		if(i == ((lista*) rc1->ciudades)->longitud){
			rc_agregarCiudad(r, ((ciudad*)iterRC2->dato)->nombre, ((ciudad*)iterRC2->dato)->poblacion);
		}
		j++;
		iterRC2 = iterRC2->siguiente;
	}

	i = 0;
	iterRC1 = ((lista*) rc1->rutas)->primero;
	while(i< ((lista*) rc1->rutas)->longitud){
		i++;
		rc_agregarRuta(r, ((ciudad*) ((ruta*)iterRC1->dato)->ciudadA)->nombre, ((ciudad*) ((ruta*)iterRC1->dato)->ciudadB)->nombre, ((ruta*)iterRC1->dato)->distancia);
		iterRC1 = iterRC1->siguiente;
	}
	j = 0;
	iterRC2 = ((lista*) rc2->rutas)->primero;
	while(j<((lista*) rc2->rutas)->longitud){
		i = 0;
		iterRC1 = ((lista*) rc1->rutas)->primero;
		while((i< ((lista*) rc1->rutas)->longitud) && !(r_cmp(((ruta*)iterRC1->dato), ((ruta*)iterRC2->dato))==0)){
			i++;
			iterRC1 = iterRC1->siguiente;
		}
		if(i == ((lista*) rc1->rutas)->longitud){
			rc_agregarRuta(r, ((ciudad*) ((ruta*)iterRC2->dato)->ciudadA)->nombre, ((ciudad*) ((ruta*)iterRC2->dato)->ciudadB)->nombre, ((ruta*)iterRC2->dato)->distancia);
		}
		j++;
		iterRC2 = iterRC2->siguiente;
	}
	return r;
}

redCaminera* rc_obtenerSubRed(char* nombre, redCaminera* rc, lista* ciudades) {
	redCaminera* r = rc_crear(nombre);
	nodo* iterLista = ciudades->primero;
	//int esta[ciudades->longitud];
	uint32_t i = 0;
	//while(i<ciudades->longitud){
	//	esta[i] = 0;
	//	i++;
	//}
	//i = 0;
	while(i < ciudades->longitud){
		uint32_t j = 0;
		nodo* iterRC = ((lista*) rc->ciudades)->primero;
		while((j< ((lista*) rc->ciudades)->longitud) && !(c_cmp(((ciudad*)iterRC->dato), ((ciudad*)iterLista->dato))==0)){
			j++;
			iterRC = iterRC->siguiente;
		}
		if(j < ((lista*) rc->ciudades)->longitud){
			rc_agregarCiudad(r, ((ciudad*)iterLista->dato)->nombre, ((ciudad*)iterLista->dato)->poblacion);
			//esta[i] = 1;
		}
		i++;
		iterLista = iterLista->siguiente;
	}
	/*
	iterLista = ciudades->primero;
	i = 0;
	while(i < ciudades->longitud){
		uint32_t j = 0;
		nodo* iterListaaux = ciudades->primero;
		while(j < ciudades->longitud){
			if((esta[i] == 1) && (esta[j]) == 1){
				ruta* rAux = obtenerRuta(rc, ((ciudad*)iterLista->dato)->nombre, ((ciudad*)iterListaaux->dato)->nombre);
				if(rAux != 0){
					rc_agregarRuta(r, ((ciudad*)iterLista->dato)->nombre, ((ciudad*)iterListaaux->dato)->nombre, rAux->distancia);
				}
				ruta* rAux2 = obtenerRuta(rc, ((ciudad*)iterListaaux->dato)->nombre, ((ciudad*)iterLista->dato)->nombre);
				if(rAux2 != 0){
					rc_agregarRuta(r, ((ciudad*)iterListaaux->dato)->nombre, ((ciudad*)iterLista->dato)->nombre, rAux->distancia);
				}
				r_borrar(rAux2);
				r_borrar(rAux);
			}
			j++;
			iterListaaux = iterListaaux->siguiente;
		}
		i++;
		iterLista = iterLista->siguiente;
	}
	*/

	lista* rutitas = ((lista*) rc->rutas);
	nodo* iter = rutitas->primero;
	i = rutitas->longitud;

	while ( i != 0){

		//printf( "ruta ciudadA : %s\n" , ((ciudad*) ((ruta*) ((nodo*) iter)->dato)->ciudadA)->nombre);
		//printf( "ruta ciudadB : %s\n" , ((ciudad*) ((ruta*) ((nodo*) iter)->dato)->ciudadB)->nombre);

		rc_agregarRuta( r, ((ciudad*) ((ruta*) ((nodo*) iter)->dato)->ciudadA)->nombre , ((ciudad*) ((ruta*) ((nodo*) iter)->dato)->ciudadB)->nombre, ((ruta*) ((nodo*) iter)->dato)->distancia);
		iter = iter->siguiente;
		i--;
	}



	return r;
}
