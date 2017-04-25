extern free
extern malloc

%define NULL 0
;Definiciones de offset de lista
%define lista_long 0
%define lista_pri 4
%define lista_ult 12
%define tam_lista 20
;Definiciones de offset de nodo
%define nodo_fun 0
%define nodo_dato 8
%define nodo_siguiente 16
%define nodo_anterior 24
%define tam_nodo 32
;Definiciones de offset de redCaminera
%define red_ciudades 0
%define red_rutas 8
%define red_nombre 16
%define tam_red 24
;Definiciones de offset de ciudad
%define ciudad_nombre 0
%define ciudad_poblacion 8
%define tam_ciudad 16
;Definiciones de offset de ruta
%define ruta_ciudadA 0
%define ruta_distancia 8
%define ruta_ciudadB 16
%define tam_ruta 24
; LISTA 

global l_crear
l_crear:
	push rbp
	mov rbp, rsp

	mov rdi, tam_lista ;Copio en rdi el tamaño de la lista
	call malloc 
	;Me devuelve el puntero en rax y asigno la longitud y los valores de los punteros
	mov dword [rax+lista_long], NULL 
	mov qword [rax+lista_pri], NULL
	mov qword [rax+lista_ult], NULL

	pop rbp
	ret

global l_agregarAdelante
l_agregarAdelante:
	push rbp
	mov rbp, rsp
	push rbx
	push r12
	push r13
	push r14
	

	mov r12, [rdi] ;Puntero a la lista
	mov rbx, [r12+lista_pri] ;me guardo el primero de la lista
	mov r13, rsi ;me guardo el puntero al dato
	mov r14, rdx ;me guardo el puntero a la funcion borrar
	;Defino el nodo
	mov rdi, tam_nodo
	call malloc
	;actualizo los valores para el nodo
	mov [rax+nodo_fun], r14
	mov [rax+nodo_dato], r13
	mov qword [rax+nodo_anterior], 0
	mov [rax+nodo_siguiente], rbx


	cmp qword [r12+lista_long], 0 ;Aca veo si es el primer elemento de la lista
	je .primerElem
	mov [rbx+nodo_anterior], rax
	;actualizo la lista, cambiando el primero y la longitud
	.primerElem:
	add dword [r12+lista_long], 1 ;Incremento la longitud de la lista
	mov [r12+lista_pri], rax
	
	;Veo si el puntero al ultimo elemento es NULL si lo es lo defino como 
	;el elemento que acabo de agregar
	cmp qword [r12+lista_ult], 0 
	je .listaVacia
	jmp .fin

	.listaVacia: ;Defino el elemento que acabo de agregar como el ultimo
		mov [r12+lista_ult], rax


	.fin:
	pop r14
	pop r13
	pop r12
	pop rbx
	pop rbp
	ret

global l_agregarAtras
l_agregarAtras:
	push rbp
	mov rbp, rsp
	push r15
	push r12
	push r13
	push r14
	

	mov r12, [rdi] ;Puntero a la lista
	mov r15, [r12+lista_ult] ;me guardo el ultimo de la lista
	mov r13, rsi ;me guardo el puntero al dato
	mov r14, rdx ;me guardo el puntero a la funcion borrar
	;Defino el nodo
	mov rdi, tam_nodo
	call malloc
	;actualizo los valores para el nodo
	mov [rax+nodo_fun], r14
	mov [rax+nodo_dato], r13
	mov qword [rax+nodo_anterior], r15
	mov qword [rax+nodo_siguiente], 0

	;Veo si la lista esta vacia
	cmp dword [r12+lista_long], 0
	je .primerElem
	mov qword [r15+nodo_siguiente], rax
	.primerElem:
	;actualizo la lista, cambiando el ultimo y la longitud
	add dword [r12+lista_long], 1
	mov [r12+lista_ult], rax
	
	cmp qword [r12+lista_pri], 0 ;Veo si el primero de la lista es NULL
	je .listaVacia ;Si lo es defino al elem que acabo de agregar como el primero
	jmp .final

	.listaVacia:
		mov [r12+lista_pri], rax

	.final:
	pop r14
	pop r13
	pop r12
	pop r15
	pop rbp
	ret

global l_agregarOrdenado
l_agregarOrdenado:
	push rbp
	mov rbp, rsp
	push r15
	push r14
	push r13
	push r12
	push rbx
	sub rsp, 8

	mov r15, [rdi] ;Me guardo el puntaro a la lista
	mov r14, rsi ;Me guardo el dato del nodo
	mov r13, rdx ;Me guardo el puntero a la funcion de borrar
	mov r12, rcx ;Me guardo el puntero a la funcion de comprar

	mov rdi, tam_nodo
	call malloc
	;Ahora en rax tengo el nuevo nodo
	mov [rax+nodo_dato], r14 ;Guardo el dato
	mov [rax+nodo_fun], r13 ;Me guardo el puntero a la funcion borrar
	mov r13, rax ;Me quedo con el nodo

	cmp qword [r15+lista_long], 0 ;Veo si la lista es vacia
	je .ListaVacia

	;Lo uso para recorrer la lista, lo inicio en 1 para no avanzar de mas
	mov ebx, [r15+lista_long]

	mov r14, [r15+lista_pri] ;Me guardo el puntero al primer elemento de la lista
	.ciclo:
		cmp ebx, 0
		;Si la longitud y mi contador son iguales salto a agregar como ultimo
		je .AgregarUltimo 
		mov rdi, [r13+nodo_dato] ;Cargo el dato pasado por parametros
		mov rsi, [r14+nodo_dato] ;Cargo el dato del nodo donde estoy parado
		;Aca llamo a la funcion de comparacion (Me aprovecho que no toco r10)
		call r12 
		cmp eax, 0
		je .Agregar ;Si son iguales lo agrego
		cmp eax, 1
		je .Agregar ;Si el dato que me pasaron es menor lo agrego tambien
		;Sino aumento mis variables
		dec rbx 
		cmp ebx, 0
		;Si la longitud y mi contador son iguales salto a agregar como ultimo
		je .AgregarUltimo
		mov r14, [r14+nodo_siguiente]
		jmp .ciclo

	.Agregar:
		mov [r13+nodo_siguiente], r14 
		cmp qword [r14+nodo_anterior], 0 ;Veo si rbx era el primero de la lista
		je .primero ;si lo era solo falta conectar la lista
		;Aca tambien conecto la lista
		mov r10, [r14+nodo_anterior] 
		mov [r13+nodo_anterior], r10 
		mov [r10+nodo_siguiente], r13
		mov [r14+nodo_anterior], r13
		jmp .fin
		.primero:
			mov [r14+nodo_anterior], r13 
			mov qword [r13+nodo_anterior], 0
			mov [r15+lista_pri], r13
			jmp .fin

	.AgregarUltimo:
		;Igual que antes me guardo el puntero al vector en rbx que ya lo use 
		mov qword [r13+nodo_siguiente], 0
		mov [r14+nodo_siguiente], r13
		mov [r13+nodo_anterior], r14
		mov [r15+lista_ult], r13
		jmp .fin

	.ListaVacia:
		;Igual que el anterior nada mas que estoy agregando a una lista vacia
		mov qword [r13+nodo_siguiente], 0
		mov qword [r13+nodo_anterior], 0
		mov [r15+lista_pri], r13
		mov [r15+lista_ult], r13
		jmp .fin

	.fin:
	inc qword [r15+lista_long] ;Aca incremento la longitud de la lista
	add rsp, 8
	pop rbx
	pop r12
	pop r13
	pop r14
	pop r15
	pop rbp
	ret

global l_borrarTodo
l_borrarTodo:
	push rbp
	mov rbp, rsp
	push r15
	push rbx
	push r13
	push r14
	push r12
	sub rsp,8

	mov r15, rdi ;Me guardo la lista
	mov ebx, 0 ;Para recorrer la lista
	
	mov r14, [r15+lista_pri] ;Me guardo el primero de la lista
	.ciclo:
		cmp ebx, [r15+lista_long]
		je .EleminarLista ;Ya recorri todos los nodos
		mov r13, [r14+nodo_fun] ;Paso el puntero a la funcion borrar
		cmp r13, 0 ;Veo que exista la funcion para borrar
		je .sigo
		mov rdi, [r14+nodo_dato]
		call r13 ;Borro el dato del nodo
		.sigo:
		mov r12, r14
		mov r14, [r14+nodo_siguiente]
		mov rdi, r12
		call free ;Libero el nodo que estaba apuntando r12
		inc rbx ;Incremento mi contador y vuelvo a llamar
		jmp .ciclo

	.EleminarLista:
	mov rdi, r15 ;Una vez que elimine todos los nodos, elimino la lista
	call free
	
	.fin:
	add rsp,8
	pop r12
	pop r14
	pop r13
	pop rbx
	pop r15
	pop rbp
	ret

; CIUDAD

global c_crear
c_crear:
	push rbp
	mov rbp, rsp
	push r15
	push r13
	

	mov r13, rsi ;Me guardo la poblacion de la ciudad
	call str_copy 
	mov r15, rax ;Me guardo el puntero al nombre de la ciudad
	;Pido el espacio para una ciudad
	mov rdi, tam_ciudad
	call malloc
	;Veo que no hubo un error al reservar la memoria
	cmp rax, 0
	je .fin3
	;Actualizo los valores de la ciudad
	mov qword [rax+ciudad_nombre], r15
	mov qword [rax+ciudad_poblacion], r13

	.fin3:
	pop r13
	pop r15
	pop rbp
	ret

global c_cmp
c_cmp:
	push rbp
	mov rbp, rsp
	push r12
	push r13

	mov r12, rdi ;Me guardo la primera ciudad
	mov r13, rsi ;Me gaurdo la segunda 
	mov rdi, [r12+ciudad_nombre] 
	mov rsi, [r13+ciudad_nombre]
 	;Paso los nombres a rdi y rsi para llamar a str_cmp
	call str_cmp
	
	pop r13
	pop r12
	pop rbp
	ret

global c_borrar
c_borrar:
	push rbp
	mov rbp, rsp
	push r13
	sub rsp, 8

	mov r13, rdi

	mov rdi, [r13+ciudad_nombre] ;Libero el nombre de la ciudad
	call free

	mov rdi, r13 ;Libero la ciudad en si, o sea el puntero y la poblacion
	call free

	add rsp, 8
	pop r13
	pop rbp
	ret

; RUTA

global r_crear
r_crear:
	push rbp
	mov rbp, rsp
	push r12
	push r13
	push r14
	sub rsp, 8

	mov r12, rdi ;Me guardo el puntero a la primera ciudad
	mov r13, rsi ;Me guardo el puntero a la segunda ciudad
	;En xmm0 tengo la distancia

	call c_cmp 
	;Ahora tengo en eax cual de las ciudades tiene que ir primero
	cmp eax, 0
	je .fin7 ;Entrada invalida, 2 ciudades con el mismo nombre
	jg .PrimeraMasChica
	jl .SegundaMasChica

	.PrimeraMasChica:
		;Reservo memoria para la estructura
		mov rdi, tam_ruta
		call malloc
		cmp rax, 0
		je .fin7
		mov [rax+ruta_ciudadA], r12 ;Aca asigno los valores para la ruta
		mov [rax+ruta_ciudadB], r13
		movsd [rax+ruta_distancia], xmm0 ;Uso el mov de double word en escala
		jmp .fin7

	.SegundaMasChica:
		;Reservo memoria para la estructura
		mov rdi, tam_ruta
		call malloc
		cmp rax, 0
		je .fin7
		mov [rax+ruta_ciudadA], r13 ;Aca hago lo mismo
		mov [rax+ruta_ciudadB], r12
		movsd [rax+ruta_distancia], xmm0
		jmp .fin7

	.fin7:
	add rsp, 8
	pop r14
	pop r13
	pop r12
	pop rbp
	ret

global r_cmp
r_cmp:
	push rbp
	mov rbp, rsp
	push r12
	push r13

	mov r12, rdi ;Me guardo el puntero a la primera ruta
	mov r13, rsi ;Me guardo el puntero a la segunda ruta

	mov rdi, [r12+ruta_ciudadA]
	mov rsi, [r13+ruta_ciudadA]
	;LLamo a c_cmp y divido en casos
	call c_cmp
	;Aca hago algo bastante similar a str_cmp
	cmp eax, 0
	je .PrueboLaSegundaCiudad
	jl .SegundaMenor
	jg .PrimeraMenor

	.SegundaMenor:
		mov eax, -1
		jmp .fin

	.PrimeraMenor:
		mov eax, 1
		jmp .fin

	.PrueboLaSegundaCiudad:
		mov rdi, [r12+ruta_ciudadB]
		mov rsi, [r13+ruta_ciudadB]
		call c_cmp
		;Hago lo mismo que arriba
		cmp eax, 0
		je .iguales
		jl .SegundaMenor
		jg .PrimeraMenor
		jmp .fin

	.iguales:
		mov eax, 0
		jmp .fin

	.fin:
	pop r13
	pop r12
	pop rbp
	ret

global r_borrar
r_borrar:
	jmp free ;Libero la estructura
ret

; RED CAMINERA

global rc_crear
rc_crear:
	push rbp
	mov rbp, rsp
	push r12
	push r15
	push r13
	sub rsp, 8

	;Tengo el nombre en rdi llamo a str_copy para definirlo en memoria dinamica
	call str_copy
	mov r12, rax ;Me lo guardo
	;Creo la listas vacias
	call l_crear
	mov r15, rax ;Me guardo la direccion a las listas vacias
	call l_crear
	mov r13, rax

	;Ahora reservo el espacio para red
	mov rdi, tam_red
	call malloc
	;Ahora asigno los valores en rax

	mov qword [rax+red_ciudades], r15
	mov qword [rax+red_rutas], r13
	mov qword [rax+red_nombre], r12

	.fin:
	add rsp, 8
	pop r13
	pop r15
	pop r12
	pop rbp
	ret

global rc_agregarCiudad
rc_agregarCiudad:
	push rbp
	mov rbp, rsp
	push r15
	push r14
	push r13
	push r12
	push rbx
	sub rsp,8

	mov r15, rdi ;Me guardo el puntero a la red
	mov r14, rsi ;Me guardo el nombre de la ciudad a agregar
	mov r13, rdx ;Me guardo la poblacion 

	mov r11, [r15+red_ciudades] ;Tengo la lista de ciudades
	mov ebx, [r11+lista_long] ;Lo uso para recorrer la lista
	mov r12, [r11+lista_pri] ;Y me guardo el puntero al primer elemento

	.ciclo:
		cmp ebx, 0
		je .Agrego ;Si recorri toda la lista y no encontre otro nombre igual lo agrego 
		mov rdi, r14 ;Muevo el nombre de la ciudad pasada por parametros
		mov r10, [r12+nodo_dato]
 		mov rsi, [r10+ciudad_nombre] ;Muevo el nombre del elemento de la lista
		call str_cmp
		cmp eax, 0 ;Los comparo si da 0 no lo agrego
		je .NoAgrego
		mov r12, [r12+nodo_siguiente] ;Luego avanzo las variables
		dec rbx
		jmp .ciclo

	.NoAgrego:
		jmp .fin ;Como no puedo agregar salto hasta el final

	.Agrego:
		;Creo la ciudad que voy a agregar
		mov rdi, r14
		mov rsi, r13
		call c_crear

		lea rdi, [r15+red_ciudades] ;Paso la lista como puntero a puntero (espero)
		mov rsi, rax ;Paso la ciudad que acabo de crear 
		mov rdx, c_borrar ;Paso la funcion borrar y la de comparacion
		mov rcx, c_cmp
		call l_agregarOrdenado ;Llamo a agregar ordenado de lista
		jmp .fin
		

	.fin:
	add rsp, 8
	pop rbx
	pop r12
	pop r13
	pop r14
	pop r15
	pop rbp
	ret

global rc_agregarRuta
rc_agregarRuta:
push rbp
	mov rbp, rsp
	push r15
	push r14
	push r13
	push r12
	push rbx
	sub rsp,8

	mov r15, rdi ;Me guardo el puntero a la red
	mov r14, rsi ;Me guardo el nombre de la primera ciudad de la ruta
	mov r13, rdx ;Me guardo el nombre de la segunda ciudad de la ruta
	;En xmm0 tengo la distancia 

	mov rdi, r13
	mov rsi, r14
	call str_cmp ;Veo que las ciudades pasadas por parametros no sean iguales
	cmp eax, 0
	je .fin

	mov r11, [r15+red_rutas] ;Tengo la lista de ciudades
	mov ebx, [r11+lista_long] ;Lo uso para recorrer la lista
	mov r12, [r11+lista_pri] ;Y me guardo el puntero al primer elemento

	;Aca veo que la ruta no exista en la red
	.ciclo:
		cmp ebx, 0
		je .Agrego ;Si recorri todo la ruta no existe entonces puedo agregarla
		;Aca veo que lo que punta r12 sea o no igual con la primera ciudad pasada por parametros
		mov rdi, r14
		mov r10, [r12+nodo_dato]
		mov r9, [r10+ruta_ciudadA]
		mov rsi, [r9+ciudad_nombre]
		call str_cmp
		cmp eax, 0
		je .compararSegunda ;Si son iguales voy a comparar la segunda ciudad de la ruta con la pasada por parametros
		mov rdi, r14 ;Hago lo mismo pero para la ciudad B
		mov r10, [r12+nodo_dato]
		mov r9, [r10+ruta_ciudadB]
		mov rsi, [r9+ciudad_nombre]
		call str_cmp
		cmp eax, 0
		je .compararSegunda ;Aca hago lo mismo
		.vuelvo:
		dec rbx
		mov r12, [r12+nodo_siguiente]
		jmp .ciclo

	;Esto lo hago porque como no se si me pasan las ciudades en orden lexicografico tengo que chequear todo
	.compararSegunda: ;El punto de la funcion es que entre por el lado que entre siempre chequeo la ruta en general
		mov rdi, r13
		mov r10, [r12+nodo_dato]
		mov r9, [r10+ruta_ciudadB]
		mov rsi, [r9+ciudad_nombre]
		call str_cmp ;Comparo la segunda ciudad de la ruta que esta siendo apuntada por r12
		cmp eax, 0
		je .fin ;Si son iguales no la agrego porque la ruta ya existe
		mov rdi, r13 ;Y hago lo mismo con la primera ciudad de la ruta
		mov r10, [r12+nodo_dato]
		mov r9, [r10+ruta_ciudadA]
		mov rsi, [r9+ciudad_nombre]
		call str_cmp
		cmp eax, 0
		je .fin
		jmp .vuelvo

	.Agrego:
		;Una vez que se que puedo agregar veo donde agregarla, tengo que crear la ruta
		mov r11, [r15+red_ciudades] ;Tengo la lista de ciudades
		mov ebx, [r11+lista_long] ;Lo uso para recorrer la lista
		mov r12, [r11+lista_pri] ;Y me guardo el puntero al primer elemento

		;Aca veo si las ciudades existen en la red
		.cicloBusca:
			cmp ebx, 0
			je .fin ;Si no encuentro la ciudad salto hasta el final sin agregar la ruta
			mov rdi, r14
			mov r10, [r12+nodo_dato]
			mov rsi, [r10+ciudad_nombre]
			call str_cmp
			cmp eax, 0
			je .BuscoLaOtra
			dec rbx
			mov r12, [r12+nodo_siguiente]
			jmp .cicloBusca

		.BuscoLaOtra:
			mov r14, [r12+nodo_dato]
			mov r11, [r15+red_ciudades] ;Tengo la lista de ciudades
			mov ebx, [r11+lista_long] ;Lo uso para recorrer la lista
			mov r12, [r11+lista_pri] ;Y me guardo el puntero al primer elemento
			;Es el mismo ciclo de antes pero con la segunda ciudad 
			.cicloBusca2:
				cmp ebx, 0
				je .fin
				mov rdi, r13
				mov r10, [r12+nodo_dato]
				mov rsi, [r10+ciudad_nombre]
				call str_cmp
				cmp eax, 0
				je .Listo
				dec rbx
				mov r12, [r12+nodo_siguiente]
				jmp .cicloBusca2

		.Listo:
			;Y si cumple todo lo anterior creo la ruta
			mov r13, [r12+nodo_dato]
			mov rdi, r14
			mov rsi, r13
			;Distancia en xmm0
			call r_crear
			;Ahora con lo creado lo agrego a la red
			lea rdi, [r15+red_rutas]
			mov rsi, rax
			mov rdx, r_borrar
			mov rcx, r_cmp
			call l_agregarOrdenado
			jmp .fin

	.fin:
	add rsp, 8
	pop rbx
	pop r12
	pop r13
	pop r14
	pop r15
	pop rbp
	ret

global rc_borrarTodo
rc_borrarTodo:
	push rbp
	mov rbp, rsp
	push r13
	sub rsp,8

	mov r13, rdi ;Me guardo la red caminera

	mov rdi, [r13+red_ciudades]
	call l_borrarTodo ;Borro la lista de ciudades

	mov rdi, [r13+red_rutas]
	call l_borrarTodo ;Borro la lista de rutas

	mov rdi, [r13+red_nombre]
	call free ;Borro el nombre de la red

	mov rdi, r13
	call free ;y despues libero la estructura

	add rsp,8
	pop r13
	pop rbp
	ret

; OTRAS DE RED CAMINERA

global obtenerCiudad
obtenerCiudad:
	push rbp
	mov rbp, rsp
	push r15
	push r14
	push r12
	push rbx

	mov r15, rdi ;red caminera
	mov r14, rsi ;nombre

	mov r12, [r15+red_ciudades] ;Me guardo la lista de ciudades
	mov ebx, [r12+lista_long] ;Me guardo la longitud de la lista para recorrerla
	mov r15, [r12+lista_pri] ;Y tengo un puntero al primer elementos

	.ciclo:
		cmp ebx, 0
		je .noExiste ;Si llego hasta aca implica que en la lista no estaba la ciudad
		;Aca comparo por el nombre
		mov r10, [r15+nodo_dato] 
		mov rdi, r14
		mov rsi, [r10+ciudad_nombre]
		call str_cmp
		cmp eax, 0
		je .listo ;Si es igual devuelvo eso, sino sigo
		mov r15, [r15+nodo_siguiente]
		dec rbx
		jmp .ciclo

	.listo:
		mov rax, r15 ;Me guardo el puntero en rax y lo devuelvo
		jmp .fin

	.noExiste:
 		mov qword rax, 0 ;Sino devuelvo NULL
 
	.fin:
	pop rbx
	pop r12
	pop r14
	pop r15
	pop rbp
	ret

global obtenerRuta
obtenerRuta:
	push rbp
	mov rbp, rsp
	push r15
	push r14
	push r13
	push r12
	push rbx
	sub rsp,8

	mov r15, rdi ;red
	mov r14, rsi ;primer nombre
	mov r13, rdx ;segundo nombre

	mov r12, [r15+red_rutas] ;Me guardo las rutas
	mov ebx, [r12+lista_long] ;La longitud
	mov r15, [r12+lista_pri] ;Y un puntero al primer elemento

	mov rdi, r14
	mov rsi, r13
	call str_cmp ;Aca veo que las ciudades no sean iguales, si lo son la ruta no existe
	cmp eax, 0
	je .noExiste

	;Nota: esto ciclo es medio complicado porque no se si me pasan las ciudades en orden lexicografico
	.ciclo:
		cmp ebx, 0
		je .noExiste ;Veo que la ruta no existe
		;Aca comparo el primer nombre pasado por paramentros contra lo que apunta r15
		mov r10, [r15+nodo_dato]
		mov rdi, r14
		mov r9, [r10+ruta_ciudadA]
		mov rsi, [r9+ciudad_nombre]
		call str_cmp
		cmp eax, 0
		je .chequeo2b ;Si esta es igual veo la segunda pasada por parametro contra la ciudadB de la ruta 
		mov r10, [r15+nodo_dato] ;Sino para la ciudadA de la ruta veo si es igual a la segunda pasada por parametro
		mov rdi, r14
		mov r9, [r10+ruta_ciudadB]
		mov rsi, [r9+ciudad_nombre]
		call str_cmp
		cmp eax, 0
		je .chequeo2a ;Si es igual veo la segunda ciudad pasada por paramentros contra la CiudadA de la ruta
		jne .avanzo
		.chequeo2b: ;Aca chequeo la segunda ciudad pasada por parametro contra la ciudad B de la ruta 
			mov r10, [r15+nodo_dato]
			mov rdi, r13
			mov r9, [r10+ruta_ciudadB]
			mov rsi, [r9+ciudad_nombre]
			call str_cmp
			cmp eax, 0
			je .listo
			jne .avanzo
		.chequeo2a: ;Lo mismo pero contra la ciudadA de la ruta
			mov r10, [r15+nodo_dato]
			mov rdi, r13
			mov r9, [r10+ruta_ciudadA]
			mov rsi, [r9+ciudad_nombre]
			call str_cmp
			cmp eax, 0
			je .listo
		.avanzo:
		dec rbx
		mov r15,[r15+nodo_siguiente]

	.listo:
		mov rax, r15 ;Si lo encuentro devuelvo ese puntero a la ciudad
		jmp .fin

	.noExiste:
		mov qword rax, 0 ;Si no lo encontre devuelvo NULL
		jmp .fin

	.fin:
	add rsp,8
	pop rbx
	pop r12
	pop r13
	pop r14
	pop r15
	pop rbp
	ret

global ciudadMasPoblada
ciudadMasPoblada:
	push rbp
	mov rbp, rsp
	push r15
	push r12
	push r13
	sub rsp,8

	mov r15, rdi ;Me guardo la red
	mov r11, [r15+red_ciudades] ;Me guardo la lista de ciudades
	mov r12, [r11+lista_pri] ;Me guardo el primero de la lista
	mov ebx, [r11+lista_long] ;Me guardo la longitud de la lista
	
	cmp ebx, 0 ;Veo que la lista no sea vacia
	je .listaVacia
	
	mov r13, [r12+nodo_dato];Lo uso para guardar el maximo

	.ciclo:
		cmp ebx, 0
		je .fin 
		mov r9, [r12+nodo_dato] ;Me guardo la ciudad que estoy viendo
		mov r8, [r13+ciudad_poblacion] ;Aca asigno a 2 registros que comparo para ver cual es mas grande
		mov r10, [r9+ciudad_poblacion]
		cmp r8, r10
		jl .actualizo ;Me quedo con el mas grande
		.vuelvo:
		mov r12, [r12+nodo_siguiente] ;Aca avanzo por la lista y vuelvo a llamar al ciclo
		dec rbx
		jmp .ciclo

	.actualizo:
		mov r13, [r12+nodo_dato] ;Voy actualizando el maximo y vuelvo al ciclo
		jmp .vuelvo


	.fin:
	mov rax, r13 ;Devuelvo el maximo
 	;Notar que si la lista es vacia el valor en rax es fruta
	.listaVacia:
	add rsp,8
	pop r13
	pop r12
	pop r15
	pop rbp
	ret

global rutaMasLarga
rutaMasLarga:
	push rbp
	mov rbp, rsp
	push r15
	push r12
	push rbx
	push r13
	push r14
	sub rsp, 8

	mov r15, rdi; Me guardo la red, la lista de rutas y algunas cosas mas
	mov r11, [r15+red_rutas]
	mov r12, [r11+lista_pri]
	mov ebx, [r11+lista_long]
	

	cmp ebx, 0 ;lista vacia
	je .listaVacia
	
	mov r13, [r12+nodo_dato] ;res

	.ciclo:
		cmp ebx, 0
		je .fin
		mov r14, [r12+nodo_dato]
		movsd xmm0, [r13+ruta_distancia]
		movsd xmm1, [r14+ruta_distancia]
		ucomisd xmm0, xmm1
		jb .actualizo
		je .veoMasChica
		.vuelvo:
		dec rbx
		mov r12, [r12+nodo_siguiente]
		jmp .ciclo

	.actualizo:
		mov r13, [r12+nodo_dato]
		jmp .vuelvo

	.veoMasChica:
		mov rdi, r13 
		mov rsi, [r12+nodo_dato]
		call r_cmp
		cmp eax, 0
		jl .actualizo
		jmp .vuelvo

	.fin:
	mov rax, r13
	.listaVacia:
	add rsp, 8
	pop r14
	pop r13
	pop rbx
	pop r12
	pop r15
	pop rbp
	ret

global ciudadesMasLejanas
ciudadesMasLejanas:
	push rbp
	mov rbp, rsp
	push r15
	push r14
	push r13
	push r12
	push rbx
	sub rsp, 8

	mov r15, rdi ;red
	lea r14, [rsi] ;puntero a la primera ciudad
	lea r13, [rdx] ;puntero a la segunda ciudad

	mov rdi, r15
	call rutaMasLarga
	mov r12, rax

	mov rbx, [r12+ruta_ciudadA]
	mov [r14], rbx
	
	mov rbx, [r12+ruta_ciudadB]
	mov [r13], rbx
	
	add rsp, 8
	pop rbx
	pop r12
	pop r13
	pop r14
	pop r15
	pop rbp
	ret

global totalDeDistancia
totalDeDistancia:
	push rbp
	mov rbp, rsp
	push r15
	push r12
	push r13
	push rbx

	mov r15, rdi;red
	mov r11, [r15+red_rutas]
	mov r12, [r11+lista_pri]
	mov ebx, [r11+lista_long]
	pxor xmm0, xmm0 ;res

	.ciclo:
		cmp ebx, 0
		je .fin
		mov r10, [r12+nodo_dato]
		movsd xmm1, [r10+ruta_distancia]
		addsd xmm0,xmm1
		dec rbx
		mov r12, [r12+nodo_siguiente]
		jmp .ciclo
	
	.fin:
	pop rbx
	pop r13
	pop r12
	pop r15
	pop rbp
	ret


global totalDePoblacion
totalDePoblacion:
	push rbp
	mov rbp, rsp
	push r15
	push r12
	push r13
	push rbx

	mov r15, rdi;red
	mov r11, [r15+red_ciudades]
	mov r12, [r11+lista_pri]
	mov ebx, [r11+lista_long]
	mov r13, 0;res

	.ciclo:
		cmp ebx, 0
		je .fin
		mov r10, [r12+nodo_dato]
		mov r9, [r10+ciudad_poblacion]
		add r13,r9
		dec rbx
		mov r12, [r12+nodo_siguiente]
		jmp .ciclo
	
	.fin:
	mov rax, r13
	pop rbx
	pop r13
	pop r12
	pop r15
	pop rbp
	ret

global cantidadDeCaminos
cantidadDeCaminos:
	push rbp
	mov rbp, rsp
	push r15
	push r14
	push r13
	push r12
	push rbx
	sub rsp, 8


	mov r15, rdi ;red
	mov r14, rsi ;nombre

	mov r11, [r15+red_rutas]
	mov r12, [r11+lista_pri]
	mov ebx, [r11+lista_long]
	mov r13d, 0;res
	
	.ciclo:
		cmp ebx, 0
		je .fin
		mov r10, [r12+nodo_dato]
		mov rdi, r14
		mov r9, [r10+ruta_ciudadA]
		mov rsi, [r9+ciudad_nombre]
		call str_cmp
		cmp eax, 0
		jne .pruebo2
		inc r13
		jmp .avanzo
		.pruebo2:
		mov r10, [r12+nodo_dato]
		mov rdi, r14
		mov r9, [r10+ruta_ciudadB]
		mov rsi, [r9+ciudad_nombre]
		call str_cmp
		cmp eax, 0
		jne .avanzo
		inc r13
		.avanzo:
		dec rbx
		mov r12, [r12+nodo_siguiente]
		jmp .ciclo

	.fin:
	xor rax, rax
	mov eax, r13d
	add rsp,8
	pop rbx
	pop r12
	pop r13
	pop r14
	pop r15
	pop rbp
	ret

global ciudadMasComunicada
ciudadMasComunicada:
	push rbp
	mov rbp,rsp
	push r15
	push rbx
	push r12
	push r13
	push r14
	sub rsp,8

	mov r15, rdi ;red
	mov r11, [r15+red_ciudades]
	mov r12, [r11+lista_pri]
	mov ebx, [r11+lista_long]
	mov r13,[r12+nodo_dato] ;res

	cmp ebx, 0
	je .listaVacia

	.ciclo:
		cmp ebx, 0
		je .fin
		mov rdi, r15
		mov rsi, [r13+ciudad_nombre]
		call cantidadDeCaminos
		mov r14, rax
		mov r9, [r12+nodo_dato]
		mov rdi, r15
		mov rsi, [r9+ciudad_nombre]
		call cantidadDeCaminos
		cmp r14, rax
		jl .actualizo
		je .veoMasChica
		.vuelvo:
		dec rbx
		mov r12, [r12+nodo_siguiente]
		jmp .ciclo

	.actualizo:
		mov r13, [r12+nodo_dato]
		jmp .vuelvo

	.veoMasChica:
		mov r9, [r12+nodo_dato]
		mov rdi, [r13+ciudad_nombre]
		mov rsi, [r9+ciudad_nombre]
		call str_cmp
		cmp eax, 0
		jl .actualizo
		jmp .vuelvo

	.fin:
	mov rax, r13
	.listaVacia:
	add rsp,8
	pop r14
	pop r13
	pop r12
	pop rbx
	pop r15
	pop rbp
	ret

; AUXILIARES

global str_copy
str_copy:
	push rbp
	mov rbp, rsp
	push r12
	push r13
	push rbx
	sub rsp, 8

	mov rbx, rdi ;Me guardo el puntero al string
	;Ahora recorro el string para saber la longitud
	mov r12d, 0
	.longitud:
		cmp byte [rdi], 0
		je .listo
		inc r12
		inc rdi
		jmp .longitud
	

	;Ahora que tengo la longitud reservo el espacio con malloc
	.listo:
		inc r12 ;incremento una vez mas r12 porque me falta considerar el 0
		mov rdi, r12
		call malloc 
		;veo que malloc funciono
		cmp rax, 0
		je .fin6

	;me guardo la longitud en r13 para actualizar rax
	mov r13, r12
	;Ahora recorro otra vez el string para copiar todos los valores en el nuevo string
	.ciclo3:
	cmp r12, 0
		je .fin6
		mov r10b,[rbx] ;me copio el valor del string 
		mov [rax],r10b ;ahora con el valor del string lo paso a mi string nuevo
		lea rax,[rax+1]
		lea rbx,[rbx+1]
		dec r12
		jmp .ciclo3

	.fin6:
 	sub rax, r13 ;Lo hago para devover a rax en la pos correcta
 	add rsp,8
 	pop rbx
 	pop r13
 	pop r12
 	pop rbp
	ret

global str_cmp
str_cmp:
	push rbp
	mov rbp, rsp
	push r13
	push r12

	.ciclo:
		mov r13b, [rdi] ;Me guardo el primer elemento del primer string
		mov r12b, [rsi] ;Me guardo el segundo elem del segundo string
		cmp r13b, 0
		je .ChequeoElSegundo
		cmp r12b, 0
		je .TerminoElSegundo
		cmp r13b, r12b
		jg .PrimeroMasGrande ;si el primero es mas grande salto
		jl .SegundoMasGrande ;si es mas chico tambien salto
		;si son iguales me muevo por el arreglo string de ambos
		inc rdi
		inc rsi
		jmp .ciclo

	.ChequeoElSegundo:
		cmp r12b, 0 ;Se que termino el primero, ahora me fijo el segundo 
		je .SonIguales ;Si termino tambien son iguales, sino el primero es mas chico
		mov eax, 1
		jmp .fin4

	.TerminoElSegundo:
		mov eax, -1 ;Como vengo de chequear que el primero no termino
		jmp .fin4 ;se que el segundo es mas chico

	.SonIguales:
		mov eax, 0 ;aca vengo de ver que ambos son cero
		jmp .fin4

	.PrimeroMasGrande: 
		mov eax, -1 ;aca si el primero tiene una letra mas grande que el segundo
		jmp .fin4

	.SegundoMasGrande:
		mov eax, 1 ;lo mismo que el caso anterio pero con el segundo mas grande
		jmp .fin4


	.fin4:
	pop r12
	pop r13
	pop rbp
	ret
