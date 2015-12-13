#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>

//definicion de variables globales
#define _CRT_NONSTDC_NO_DEPRECATE // permite utilizar funciones deprecadas
#define BUFFER_SIZE 1024 // tamaño maximo de una linea del documento
#define NUM_EQUIPOS 12 // maximo numero de equipos, no puede ser mayor que 12
#define MAX_ANIO 2015 // año del ultimo campeonato
#define NUM_ANIOS 2 // numero de torneos que se van a representar
#define NUM_TEMPORADAS 2 // numero de temporadas en un año, solo puede ser 1 o 2
#define PARTIDOS_TEMP 22 // numero de partidos por temporada

void menuPrincipal(); // funcion que imprime el menu principal con las opciones
void generarDatos(char *listaEquipos[], FILE *datos); // funcion que se encarga de llenar campeonatos.dat
const char* extraerValor(char* line, int num); // funcion que me permite extraer un parametro de una linea del archivo
int cargarInformacion(FILE *datos, struct Campeonatos campeonato[NUM_ANIOS]); // funcion que me permite cargar los datos del archivo en la estructura de datos, retorna el tamaño de la estructura
struct Campeonatos* campeonatosJugados(char *equipo, struct Campeonatos *campeonato, int tam); // funcion que retorna una lista de campeonatos en el que el equipo participo
void guardaInformacionEquipo(char* equipo, struct Campeonatos *campeonato, int tam); // funcion que me permite guardar los datos de un equipo
int numeroAleatorio(int minimo, int maximo); // funcion que retorna numero aleatorio entre un rango de valores
void reordenar(int *array, size_t n); // funcion que reordena valores de un arreglo
int compararNombres(char *equipo1, char *equipo2); // funcion que compara dos strings y retorna 0 cuando son iguales

typedef struct Equipos{ // defino estructura para los equipos
	char *nombre; // nombre del equipo
	int pos, pj, pg, pe, pp, gf, gc; // partidos jugados, ganados, empatados, perdidos, gol favor, gol contra
} Equipo;

typedef struct Campeonatos{ // defino estructura para los campeonatos
	int anio; // año del campeonato
	char* temporada; // lista de temporadas en un campeonato
	Equipo equipo;
} Campeonato;

int main()
{
	srand(time(NULL));
	int opcion = 0; int tam = 0;
	Campeonato campeonatos[NUM_ANIOS*NUM_TEMPORADAS*NUM_EQUIPOS]; // defino mi estructura de datos
	char *listaEquipos[] = { "Barcelona", "Emelec", "LigaQuito", "UCatolica", "Independiente", "Aucas", "DCuenca",
		"Nacional", "DQuito", "Aucas", "River", "LigaLoja" }; // lista de nombres de equipos
	FILE *datos; // defino variable para leer/escribir un archivo
	FILE *datos1; // defino variable archivo
	datos = fopen("campeonatos.dat", "w"); // archivo listo para escritura
	if (datos == NULL){
		printf("Error!");
		exit(1);
	}
	generarDatos(listaEquipos, datos); // genero el arhivo campeonatos.dat
	fclose(datos); // cierro el archivo campeonatos.dat despues de la escritura
	while (opcion != 5){
		menuPrincipal();
		scanf_s("%d", &opcion);
		switch (opcion){
		case 1:
			system("CLS");
			datos1 = fopen("campeonatos.dat", "r"); // archivo listo para lectura
			if (datos1 == NULL){
				printf("Error!");
				exit(1);
			}
			tam = cargarInformacion(datos1, &campeonatos); // cargo la informacion del archivo en mi estructura de datos
			fclose(datos1);
			printf("Informacion cargada.\n");
			break;
		case 2:
			
			system("CLS");
			char equipo[100];
			printf("Ingrese nombre del equipo: ");
			scanf("%s", equipo);
			system("CLS");
			guardaInformacionEquipo(equipo, &campeonatos, tam);
			break;
		case 3:
			system("CLS");
			Campeonato *campsJugados;
			char equipo1[100];
			printf("Ingrese nombre del equipo: ");
			scanf("%s", equipo1);
			system("CLS");
			campsJugados = campeonatosJugados(equipo1, &campeonatos, tam);
			break;
		case 4:
			system("CLS");
			printf("Saliendo del simulador...\n");
			Sleep(2000);			// El programa espera 2 segundos antes de salir
			exit(EXIT_SUCCESS);		// Salir del programa exitosamente
			break;
		default:
			break;
		}
	}
}

void menuPrincipal(){
	printf("\nSimulador Campeonato Ecuatoriano v1.0\n");
	printf("\n1. Cargar informacion.\n");
	printf("2. Guardar informacion de equipo.\n");
	printf("3. Imprimir datos de un equipo.\n");
	printf("4. Salir.\n");
	printf("Ingrese opcion: ");
}

void generarDatos(char *listaEquipos[], FILE *datos){
	int x;
	int t = 0;
	int posiciones[NUM_EQUIPOS];
	

	char *temporadas[] = { "Verano", "Invierno" }; // defino las temporadas que existen

	for (int i = 0; i < NUM_ANIOS; i++){ // itero numero de años
		for (int j = 0; j < NUM_TEMPORADAS; j++){ // itero numero de temporadas
			// numeros para tabla de posiciones
			for (int k = 0; k < NUM_EQUIPOS; k++){ // itero numero de equipos
				while (t<NUM_EQUIPOS){ // genero posicion en tabla
					int r = numeroAleatorio(1, NUM_EQUIPOS);
					for (x = 0; x < t; x++)
					{
						if (posiciones[x] == r){
							
							break;
						}
					}
					if (x == t){
						posiciones[t++] = r;
					}
				}
				int pg, pp, pe; // defino variables de partidos ganados, perdidos y empatados
				int gf, gc; // defino variables para goles a favor y en contra
				pg = PARTIDOS_TEMP - (rand() % (PARTIDOS_TEMP)); // partidos ganados
				int maxpp = PARTIDOS_TEMP - pg; // maximo numero de partidos perdidos posibles
				if (maxpp == 0){ // partidos perdidos
					pp = 0;
				}
				else{
					pp = maxpp - (rand() % (maxpp));
				}
				pe = PARTIDOS_TEMP - pg - pp; // partidos empatados (diferencia)
				gf = pg + ((pg+1)) - (rand() % ((pg+1))); // goles a favor (en funcion de partidos ganados)
				gc = pp + ((pp+1)) - (rand() % ((pp+1))); // goles en contra (en funcion de partidos perdidos)

				// escribo los datos generados en el archivo
				fprintf(datos, "%d;%s;%s;%d;%d;%d;%d;%d;%d;%d;\n", (MAX_ANIO - i), temporadas[j], listaEquipos[k], posiciones[k], PARTIDOS_TEMP, pg, pe, pp, gf, gc);
			}
			reordenar(posiciones, NUM_EQUIPOS); // reordeno posiciones para simular aleatoriedad
		}
	}
}

const char* extraerValor(char* line, int num)
{
	const char* token; // variable que tomara el valor del campo que deseo extraer
	for (token = strtok(line, ";"); // defino el separador como un punto y coma
		token && *token;
		token = strtok(NULL, ";\n")) // defino el fin de una linea del archivo
	{
		if (!--num)
			return token; // retorno el valor extraido de la posicion deseada
	}
	return NULL;
}

int cargarInformacion(FILE *datos, Campeonato  *campeonato){
	int i = 0; // variable para iterar las lineas del archivo
	int equipo = 0; // variable para identificar el numero del equipo por temporada
	char line[BUFFER_SIZE]; // defino una linea del archivo con su tamaño correspondiente
	char* campos[11]; // arreglo para extraer campos de las lineas del archivo

	while (fgets(line, sizeof(line), datos)) // mientras hayan lineas en el archivo
	{
		equipo++;
		for (int j = 1; j <= 10; j++){ // iteracion de todos los campos
			campos[j] = _strdup(line);
			campos[j] = extraerValor(campos[j], j); // extraigo campo en posicion j
		}

		// asignacion de campos en estructura de datos
		campeonato[i].anio = atoi(campos[1]); // año
		campeonato[i].temporada = campos[2]; // temporada
		campeonato[i].equipo.nombre = campos[3]; // nombre del equipo
		campeonato[i].equipo.pos = atoi(campos[4]); // posicion en tabla
		campeonato[i].equipo.pj = atoi(campos[5]); // partidos jugados
		campeonato[i].equipo.pg = atoi(campos[6]); // partidos ganados
		campeonato[i].equipo.pe = atoi(campos[7]); // partidos empatados
		campeonato[i].equipo.pp = atoi(campos[8]); // partidos perdidos
		campeonato[i].equipo.gf = atoi(campos[9]); // goles a favor
		campeonato[i].equipo.gc = atoi(campos[10]); // goles en contra
		
		// muestro la estructura de datos
		/*
		printf("\nAño: %d\n", campeonato[i].anio);
		printf("Equipo: %d\n", equipo);
		printf("Temporada: %s\n", campeonato[i].temporada);
		printf("Equipo: %s\n", campeonato[i].equipo.nombre);
		printf("POS: %d, PJ: %d, PG: %d, PE: %d, PP: %d, GF: %d, GC: %d\n", campeonato[i].equipo.pos, campeonato[i].equipo.pj, campeonato[i].equipo.pg, campeonato[i].equipo.pe, campeonato[i].equipo.pp, campeonato[i].equipo.gf, campeonato[i].equipo.gc);
		*/

		if (equipo >= NUM_EQUIPOS) // se han contado todos los equipos de esta temporada, reinicia contador
			equipo = 0; 
		i++; // iteracion de linea
	}
	return i;
}


struct Campeonatos* campeonatosJugados(char *equipo, struct Campeonatos  *campeonato, int tam){
	Campeonato listCamp[NUM_ANIOS*NUM_TEMPORADAS]; // lista de campeonatos en el que participo el equipo elegido
	int cont = 0;
	for (int i = 0; i < tam; i++){
		if (compararNombres(equipo, campeonato[i].equipo.nombre) == 0){ // si el nombre ingresado coincide
			listCamp[cont] = campeonato[i]; // asigna campeonato correspondiente a la lista
			cont++;
		}
	}
	for (int i = 0; i < cont; i++){ // imprimo las participaciones del equipo
		printf("\n%s %d\n", listCamp[i].temporada, listCamp[i].anio);
		printf("Posicion en la tabla: %d\n", listCamp[i].equipo.pos);
		printf("Partidos Jugados: %d\n", listCamp[i].equipo.pj);
		printf("Partidos Ganados: %d\n", listCamp[i].equipo.pg);
		printf("Partidos Empatados: %d\n", listCamp[i].equipo.pe);
		printf("Partidos Perdidos: %d\n", listCamp[i].equipo.pp);
		printf("Goles a Favor: %d\n", listCamp[i].equipo.gf);
		printf("Goles en Contra: %d\n", listCamp[i].equipo.gc);
		printf("Gol Diferencia: %d\n", (listCamp[i].equipo.gf - listCamp[i].equipo.gc));
	}
	
	return listCamp; // retorno lista de participaciones del equipo
}

void guardaInformacionEquipo(char* equipo, struct Campeonatos *campeonato, int tam){
	int contTemps = 0; // contador de temporadas
	int sumPJ = 0, sumPG = 0, sumPE = 0, sumPP = 0, sumGF = 0, sumGC=0; // acumuladores de datos por temporada

	//concatenacion de strings para armar el nombre del archivo
	char* dat = ".dat";
	char* file = malloc(strlen(equipo) + strlen(dat) + 2);
	strcpy(file, equipo);
	strcat(file, dat);

	FILE* infoEquipo = fopen(file, "w"); // archivo listo para escritura
	if (file == NULL){
		printf("Error!");
		exit(1);
	}

	for (int i = 0; i < tam; i++){
		if (compararNombres(equipo, campeonato[i].equipo.nombre) == 0){ // si el nombre ingresado coincide
			// sumatoria de datos por temporada
			sumPJ += campeonato[i].equipo.pj;
			sumPG += campeonato[i].equipo.pg;
			sumPE += campeonato[i].equipo.pe;
			sumPP += campeonato[i].equipo.pp;
			sumGF += campeonato[i].equipo.gf;
			sumGC += campeonato[i].equipo.gc;
			contTemps++;
		}
	}
	// escribo los datos en el archivo y lo cierro
	fprintf(infoEquipo, "%d;%d;%d;%d;%d;%d;%d;%d;\n", contTemps, sumPJ, sumPG, sumPE, sumPP, sumGF, sumGC, (sumGF-sumGC));
	fclose(infoEquipo);
	printf("Archivo: '%s' ha sido creado.\n", file);
}

int numeroAleatorio(int minimo, int maximo){
	return rand() % (maximo - minimo + 1) + minimo;
}

int compararNombres(char *equipo1, char *equipo2){
	return strcmp(equipo1, equipo2);
}

void reordenar(int *array, size_t n){
	if (n > 1) {
		size_t i;
		for (i = 0; i < n - 1; i++) {
			size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
			int t = array[j];
			array[j] = array[i];
			array[i] = t;
		}
	}
}