#include "FileSistem.h"

void mainFileSistem()
{

}

void setearValoresFileSistem(t_config * archivoConfig)
{
	punto_montaje = config_get_string_value(archivoConfig, "PUNTO_MONTAJE");
//	crearTabla("TablaA", "SC", 5, 6000);
}

void crearTabla(char* nombre, char* consistencia, int particiones, int tiempoCompactacion)
{
	DIR* newdir;
	char buff[128];
	char* tablename = string_new();
	char* puntodemontaje = string_new();
	strcpy(puntodemontaje, punto_montaje);
	strcpy(tablename, nombre);
	memset(buff,0,sizeof(buff));
	strcat(puntodemontaje, "Tables/");
	strcpy(buff, puntodemontaje);


	if(NULL == (newdir = opendir(puntodemontaje)))// reviso si el punto de montaje es accesible
	{
		log_info(loggerLFL,"FileSistem: El directorio que usted desea crear no es accesible");
		exit(1);
	}
	else
	{
		char* direccionFinal = string_new();
		strcat(tablename,"/");
		strncpy(buff + strlen(buff), tablename, strlen(tablename));
		strcat(puntodemontaje, tablename);
		strncpy(direccionFinal, puntodemontaje, strlen(puntodemontaje));

		log_info(loggerLFL, "FileSistem: Se procede a la creacion del directorio");
		mkdir(buff, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH); //creo el directorio de la tabla con sus respectivos permisos
		int results = crearMetadata(direccionFinal, consistencia, particiones, tiempoCompactacion); //Crea el archivio metadata
		if (results == 1)
		{
			log_info(loggerLFL, "FileSistem: Error al crear tabla, abortando");
			closedir(newdir);
			exit(1);
		}
		else
		{
			int resultsb = crearParticiones(direccionFinal, particiones); //Crea archivos .bin
			if(resultsb == 1)
			{
				log_info(loggerLFL, "FileSistem: Error al crear tabla, abortando");
				closedir(newdir);
				exit(1);
			}
			else
				closedir(newdir);
		}
		free(direccionFinal);
	}

}

int crearMetadata (char* direccion, char* consistencia, int particiones, int tiempoCompactacion)
{
	char* direccionaux = string_new();
	strcpy(direccionaux, direccion);
	char* direccionDelMetadata = string_new();
	FILE* metadata;
	direccionDelMetadata = strcat(direccionaux, "Metadata.cfg");
	metadata = fopen(direccionDelMetadata, "w+");
	if(metadata == NULL)
	{
		log_info(loggerLFL,"FileSistem: No se pudo crear el archivo Metadata");
		return 1;
	}
	else
	{
		char* Linea = string_new();
		Linea = malloc(17);
		strcpy(Linea, "CONSISTENCIA=");
		strcat(Linea, consistencia);
		strcat(Linea,"\n");
		fwrite(Linea, strlen(Linea), 1, metadata);
		free(Linea);
		char* Linea2 = string_new();
		char* cantparticiones = string_new();
		Linea2 = malloc(sizeof(particiones) + 14);
		strcpy(Linea2, "PARTICIONES=");
		cantparticiones = string_itoa(particiones);
		strcat(Linea2, cantparticiones);
		strcat(Linea2, "\n");
		fwrite(Linea2, strlen(Linea2), 1, metadata);
		free(Linea2);
		char* Linea3 = string_new();
		char* tiempoEntreCompactaciones = string_new();
		Linea3 = malloc(sizeof(tiempoCompactacion) + 28);
		strcpy(Linea3, "TIEMPOENTRECOMPACTACIONES=");
		tiempoEntreCompactaciones = string_itoa(tiempoCompactacion);
		strcat(Linea3, tiempoEntreCompactaciones);
		strcat(Linea3, "\n");
		fwrite(Linea3, strlen(Linea3), 1, metadata);
		fclose(metadata);
		return 0;
	}
}

int crearParticiones(char* direccionFinal, int particiones)
{
	int i = 0;
	FILE* particion;
	while(i < particiones)
	{
		char* particionado = string_new();
		char* aux = string_new();
		particionado = malloc(strlen(direccionFinal)+sizeof(i)+4);
		strcpy(aux, string_itoa(i));
		strcpy(particionado, direccionFinal);
		strcat(particionado, aux);
		strcat(particionado, ".bin");
		particion = fopen(particionado, "w+");
		if(particion == NULL)
			return 1;
		else
		{
			char* size = string_new();
			size = malloc(7);
			strcpy(size, "SIZE=");
			strcat(size, "\n");
			fwrite(size, strlen(size), 1, particion);
			char* blocks = string_new();
			blocks = malloc (9);
			strcpy(blocks, "BLOCKS=");
			strcat(blocks, "\n");
			fwrite(blocks, strlen(blocks), 1, particion);
			free(particionado);
			fclose(particion);
		}
		i++;
	}
	return 0;
}

