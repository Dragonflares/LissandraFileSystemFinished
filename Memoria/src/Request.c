#include "Request.h"


void selectt (char** args) {
	char* nombre_tabla = string_duplicate(args[1]);
	uint16_t key = atoi(args[2]);
	//t_pagina* pagina_a_buscar = estaTablaYkeyEnMemoria(nombre_tabla, key);

	t_segmento* segmento_buscado = buscarSegmento(nombre_tabla);

	if(segmento_buscado /*!= NULL*/){
		t_est_pag* est_pagina_buscada = buscarEstPagBuscada(key, segmento_buscado);
		t_pagina* pagina_buscada = est_pagina_buscada->pagina;

		if(pagina_buscada /*!= NULL*/){
			//se lo mando al Kernel si esta
			prot_enviar_mensaje(socket_kernel, KEY_SOLICITADA_SELECT, strlen(pagina_buscada->value), (pagina_buscada->value));
		}
		//no se encuentra la pagina
		else{
			free(pagina_buscada);
			//se lo pido al fs porque no esta
			prot_enviar_mensaje(socket_fs, SOLICITUD_TABLA, sizeof(uint16_t), &key);
			t_prot_mensaje* mensaje_con_tabla = prot_recibir_mensaje(socket_fs);

			//el fs nos manda tiempo + tamanio value + value
			time_t tiempo_de_pag;
			int tamanio_value;

			memcpy(&tiempo_de_pag, mensaje_con_tabla->payload, sizeof(time_t));
			memcpy(&tamanio_value, mensaje_con_tabla->payload+sizeof(time_t), sizeof(int));

			char* value = malloc(tamanio_value);
			memcpy(value, mensaje_con_tabla->payload+sizeof(time_t)+sizeof(int), tamanio_value);

			//prot_destruir_mensaje

			t_est_pag* nueva_est_pag = crearPagina(tiempo_de_pag, key, tamanio_value, value);
			chequearLugaresEinsertar(segmento_buscado, nueva_est_pag);
		}
	}
	//no se encuentra el segmento
	else{
		free(segmento_buscado);
		//creo segmento
		t_segmento* segmento_nuevo = malloc(sizeof(t_segmento));
		segmento_nuevo->nombre_tabla = strdup(nombre_tabla);
		segmento_nuevo->tabla_paginas.paginas = list_create();

		//pido fs la data
		//se lo pido al fs porque no esta
		prot_enviar_mensaje(socket_fs, SOLICITUD_TABLA, sizeof(uint16_t), &key);
		t_prot_mensaje* mensaje_con_tabla = prot_recibir_mensaje(socket_fs);

		//el fs nos manda tiempo + tamanio value + value
		time_t tiempo_de_pag;
		int tamanio_value;

		memcpy(&tiempo_de_pag, mensaje_con_tabla->payload, sizeof(time_t));
		memcpy(&tamanio_value, mensaje_con_tabla->payload+sizeof(time_t), sizeof(int));

		char* value = malloc(tamanio_value);
		memcpy(value, mensaje_con_tabla->payload+sizeof(time_t)+sizeof(int), tamanio_value);

		//creo pagina
		t_est_pag* nueva_est_pag = crearPagina(tiempo_de_pag, key, tamanio_value, value);
		//chequeo lugar e inserto
		chequearLugaresEinsertar(segmento_nuevo, nueva_est_pag);
	}
}

void insert (char** args) {





}
void create (char** args) {





}
void describe (char** args) {





}
void drop (char** args) {





}
void journal (char** args) {





}
void add (char** args) {





}
void run (char** args) {





}
void readLatency (char** args) {





}
void writeLatency (char** args) {





}
void reads (char** args) {





}
void writes (char** args) {





}
void memoryLoad (char** args) {





}


/*buscarsegmento
if(segmentoBuscado){
	buscarpagina
	if(paginaBuscada){
		envio value
	}
	else{
		creoPagina
		pido al fs data
		chequearLugaresEinsertar
	}
}
else{
	creoSegmento
	creoPagina
	pido al fs data
	chequearLugaresEinsertar
}

funcion para creacion de pagina*/