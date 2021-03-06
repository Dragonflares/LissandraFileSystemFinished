#ifndef LFLEXTERNALS_H_
#define LFLEXTERNALS_H_

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <commons/config.h>
#include <commons/log.h>
#include <pthread.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <dirent.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <commons/collections/list.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <commons/string.h>
#include <time.h>
#include <ftw.h>
#include <fts.h>
#include "../../SharedLibrary/auxiliaryFunctions.h"
#include "../../SharedLibrary/loggers.h"
#include "../../SharedLibrary/conexiones.h"
#include "../../SharedLibrary/configs.h"
#include "../../SharedLibrary/protocolo.h"
#include <commons/bitarray.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/inotify.h>

/* ESTRUCTURAS */
typedef struct {
	double timestamp;
	uint16_t key;
	char* clave;
}__attribute__((packed)) t_keysetter;

typedef struct {
	t_keysetter* data;
	char* tabla;
}__attribute__((packed)) t_Memtablekeys;

typedef struct {
	char* tabla;
	int cantTemps;
	pthread_mutex_t compactacionActiva;
	pthread_mutex_t renombreEnCurso;
	pthread_mutex_t dropPendiente;
}__attribute__((packed)) t_TablaEnEjecucion;

/* VARIABLES GLOBALES */
int slowestCompactationInterval;
int tiempoDump;
char* lissandraFL_config_ruta;
bool fileSystemFull;
bool criticalFailure;
int tamanio_value;
char* direccionFileSystemBlocks;
char* globalBitmapPath;
char* bitarraycontent;
int retardo;
int blocks;
int bitarrayfd;
t_bitarray* bitarray;
t_log * logger;
bool signalExit;
t_list * memtable;
char* punto_montaje;
t_list* tablasEnEjecucion;
pthread_mutex_t deathProtocol;
pthread_mutex_t dumpEnCurso;
pthread_mutex_t modifierBitArray;
pthread_mutex_t modifierTablasEnCurso;

/* FUNCIONES GLOBALES */
int cantidadDeBloquesLibres();
int cantidadDeBloquesAOcupar();
char* timeForLogs();
int chequearTimestamps(t_Memtablekeys* key1, t_Memtablekeys* key2);
int chequearTimeKey(t_keysetter* key1, t_keysetter* key2);
int obtenerTamanioArchivoConfig(char* direccionArchivo);
t_keysetter* construirKeysetter(char* timestamp, char* key, char* value);
unsigned long obtenerTamanioArchivo(char* direccionArchivo);
t_list* parsearKeys(t_list* clavesAParsear);
t_list* inversaParsearKeys(t_list* clavesADesparsear);
int cantBloquesFS(char* direccion);
void liberadorDeKeys(t_keysetter* keysetter);
void liberadorDeMemtableKeys(t_Memtablekeys* memtableKey);
void liberadorDeListasDeKeys(t_list* listaDeKeys);


#endif /* LFLEXTERNALS_H_ */
