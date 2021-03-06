#include "pokedex.h"
#include "lista.h"
#include "abb.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ERROR -1
#define EXITO 0
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void muestra_opciones_basicas(){
    printf(ANSI_COLOR_YELLOW"\t/***MENU***/\n");
    printf("\n\n");
    printf("Iniciar Pokedex (Tecla I)\n");
    printf("Salir del programa (Tecla S)\n");
    printf("Ayuda (Tecla H)\n");
    printf("/*********************************/\n"ANSI_COLOR_RESET);
}

void muestra_todas_las_opciones(){
    printf(ANSI_COLOR_CYAN"\t/***MENU***/\n");
    printf("\n\n");
    printf("Iniciar Pokedex (Tecla I)\n");
    printf("Guardar Pokedex (Tecla G)\n");
    printf("Salir del programa (Tecla S)\n");
    printf("Ayuda (Tecla H)\n");
    printf("Avistar pokemon (Tecla A)\n");
    printf("Evolucionar Pokemon (Tecla E)\n");
    printf("Capturas Recientes (Tecla C)\n");
    printf("Vistas recientes (Tecla V)\n");
    printf("Informacion especie (Tecla M)\n");
    printf("Informacion Pokemon (Tecla P)\n");
    printf("/*********************************/\n"ANSI_COLOR_RESET );
}

void descripcion_basica(){
    printf(ANSI_COLOR_GREEN"\tOpciones validas:\n"ANSI_COLOR_RESET);
    printf(ANSI_COLOR_CYAN"Iniciar Pokedex (tecla I): Da inicio a la Pokedex, lee informacion del archivo pokedex.txt\n");
    printf("Salir del programa (tecla S): Da por finalizado el programa y libera la memoria que se encuentra reservada.\n");
    printf("Ayuda (tecla H): Brinda una breve descripcion sobre los comandos validos actuales.\n"ANSI_COLOR_RESET);
    printf(ANSI_COLOR_YELLOW"/***********************************/\n"ANSI_COLOR_RESET);
}

void descripcion_completa(){
    printf(ANSI_COLOR_GREEN"\tOpciones validas:\n"ANSI_COLOR_RESET);
    printf(ANSI_COLOR_YELLOW"Guardar Pokedex (tecla G): almacena el estado actual de la pokedex en el archivo pokedex.txt.\n");
    printf("Salir del programa (tecla S): Da por finalizado el programa y libera la memoria que se encuentra reservada.\n");
    printf("Ayuda (tecla H): Brinda una breve descripcion sobre los comandos validos actuales.\n");
    printf("Avistar Pokémon (tecla A): Incorpora a la pokedex los pokemones presentes en el archivo avistamientos.txt.\n");
    printf("Evolucionar Pokemon (Tecla E): Actualiza a los pokemones validos para evolucionar presentes en evoluciones.txt.\n");
    printf("Capturas Recientes (Tecla C): Muestra un listado de los ultimos pokemones capturados, luego de su uso vacia ese registro.\n");
    printf("Vistas recientes (Tecla V): Muestra un listado de los ultimos pokemones vistos, luego de ser consultado vacia el listado.\n");
    printf("Informacion especie (Tecla M): Brinda informacion sobre la especie buscada.\n");
    printf("Informacion Pokemon (Tecla P): Brinda informacion sobre el pokemon y su especie.\n"ANSI_COLOR_RESET);
    printf(ANSI_COLOR_YELLOW"/***********************************/\n"ANSI_COLOR_RESET);
    printf(ANSI_COLOR_CYAN"Iniciar Pokedex (tecla I): Da inicio a la Pokedex, lee informacion del archivo pokedex.txt"ANSI_COLOR_RESET);
    printf(ANSI_COLOR_RED"\t No valido, pokedex ya iniciada.\n"ANSI_COLOR_RESET);
    printf(ANSI_COLOR_YELLOW"/***********************************/\n"ANSI_COLOR_RESET);
}

int main(){
    pokedex_t* pokedex = NULL;
    bool menu_extendido = false;
    bool quiere_salir = false;
    char letra = 'a';
    char nombre_pokemon[MAX_NOMBRE];
    int especie = 0;
    int estado = 0;
    system("clear");
    while(!quiere_salir){
        if(!menu_extendido){
            muestra_opciones_basicas();
            scanf(" %c", &letra);
            while(letra != 'I' && letra != 'S' && letra != 'H'){
                printf(ANSI_COLOR_RED"La letra insertada no corresponde a una opcion valida, vuelva a intentarlo.\n"ANSI_COLOR_RESET);
                scanf(" %c", &letra);
            }
            system("clear");
            if(letra == 'H'){
                /////////////////////////////////////////////////////////////////////////////////////
                descripcion_basica();
            }else if(letra == 'I'){
                /////////////////////////////////////////////////////////////////////////////////////
                if(pokedex){
                    pokedex_destruir(pokedex);
                }
                pokedex = pokedex_prender();
                if(pokedex){
                    menu_extendido = true;
                }else{
                    printf(ANSI_COLOR_RED"Error al encerder la pokedex, falta el archivo pokedex.txt.\n"ANSI_COLOR_RESET);
                }
                printf(ANSI_COLOR_YELLOW"/***********************************/\n"ANSI_COLOR_RESET);
                /////////////////////////////////////////////////////////////////////////////////////
            }else{
                /////////////////////////////////////////////////////////////////////////////////////
                pokedex_destruir(pokedex);
                quiere_salir = true;
                /////////////////////////////////////////////////////////////////////////////////////
            }
        }else{
            muestra_todas_las_opciones();
            scanf(" %c", &letra);
            while(letra != 'I' && letra != 'G' && letra != 'S' && letra != 'H' && letra != 'A'&& letra != 'E'&& letra != 'C'&& letra != 'V'&& letra != 'M'&& letra != 'P'){
                printf(ANSI_COLOR_RED"La letra insertada no corresponde a una opcion valida, vuelva a intentarlo.\n"ANSI_COLOR_RESET);
                scanf(" %c", &letra);
            }
            system("clear");
            if(letra == 'H'){
                /////////////////////////////////////////////////////////////////////////////////////
                descripcion_completa();
                /////////////////////////////////////////////////////////////////////////////////////
            }else if(letra == 'I'){
                /////////////////////////////////////////////////////////////////////////////////////
                    printf(ANSI_COLOR_RED"La pokedex ya se encuentra iniciada.\n"ANSI_COLOR_RESET);
                    printf(ANSI_COLOR_YELLOW"/***********************************/\n"ANSI_COLOR_RESET);
                /////////////////////////////////////////////////////////////////////////////////////
            }else if(letra == 'S'){
                /////////////////////////////////////////////////////////////////////////////////////
                pokedex_destruir(pokedex);
                quiere_salir = true;
                /////////////////////////////////////////////////////////////////////////////////////
            }else if(letra == 'G'){
                /////////////////////////////////////////////////////////////////////////////////////
                estado = pokedex_apagar(pokedex);
                if(estado == EXITO){
                    printf(ANSI_COLOR_GREEN"Informacion guardada exitosamente!\n"ANSI_COLOR_RESET);
                    printf(ANSI_COLOR_YELLOW"/***********************************/\n"ANSI_COLOR_RESET);
                    menu_extendido = false;
                }else{
                    printf(ANSI_COLOR_RED"Error al guardar informacion!\n"ANSI_COLOR_RESET);
                    printf(ANSI_COLOR_YELLOW"/***********************************/\n"ANSI_COLOR_RESET);
                }
                /////////////////////////////////////////////////////////////////////////////////////
            }else if(letra == 'A'){
                /////////////////////////////////////////////////////////////////////////////////////
                estado = pokedex_avistar(pokedex, "avistamientos.txt");
                if(estado == EXITO){
                    printf(ANSI_COLOR_GREEN"Pokemones avistados correctamente!\n"ANSI_COLOR_RESET);
                    printf(ANSI_COLOR_YELLOW"/***********************************/\n"ANSI_COLOR_RESET);
                }else{
                    printf(ANSI_COLOR_RED"Error al avistar los pokemones!\n"ANSI_COLOR_RESET);
                    printf(ANSI_COLOR_YELLOW"/***********************************/\n"ANSI_COLOR_RESET);
                }
                /////////////////////////////////////////////////////////////////////////////////////
            }else if(letra == 'E'){
                /////////////////////////////////////////////////////////////////////////////////////
                estado = pokedex_evolucionar(pokedex, "evoluciones.txt");
                if(estado == EXITO){
                    printf(ANSI_COLOR_GREEN"Pokemones evolucionados exitosamente!\n"ANSI_COLOR_RESET);
                    printf(ANSI_COLOR_YELLOW"/***********************************/\n"ANSI_COLOR_RESET);
                }else{
                    printf(ANSI_COLOR_RED"Error al evolucionar algunos de los pokemones!\n"ANSI_COLOR_RESET);
                    printf(ANSI_COLOR_RED"Solo pudieron evolucionarse a los pokemones validos!\n"ANSI_COLOR_RESET);
                    printf(ANSI_COLOR_YELLOW"/***********************************/\n"ANSI_COLOR_RESET);
                }
                /////////////////////////////////////////////////////////////////////////////////////
            }else if(letra == 'C'){
                /////////////////////////////////////////////////////////////////////////////////////
                
                if(pokedex->ultimos_capturados){
                    printf(ANSI_COLOR_GREEN"Ultimos pokemones capturados:\n"ANSI_COLOR_RESET);
                    pokedex_ultimos_capturados(pokedex);
                    printf(ANSI_COLOR_YELLOW"/***********************************/\n"ANSI_COLOR_RESET);
                }else{
                    printf(ANSI_COLOR_RED"La pila se encuentra vacia, se reviso recientemente o no se ha avistado nuevos pokemones.\n"ANSI_COLOR_RESET);
                    printf(ANSI_COLOR_YELLOW"/***********************************/\n"ANSI_COLOR_RESET);
                }
                /////////////////////////////////////////////////////////////////////////////////////
            }else if(letra == 'V'){
                /////////////////////////////////////////////////////////////////////////////////////
                if(pokedex->ultimos_vistos){
                    printf(ANSI_COLOR_GREEN"Ultimos pokemones visto:\n"ANSI_COLOR_RESET);
                    pokedex_ultimos_vistos(pokedex);
                    printf(ANSI_COLOR_YELLOW"/***********************************/\n"ANSI_COLOR_RESET);
                }else{
                    printf(ANSI_COLOR_RED"La cola se encuentra vacia, se reviso recientemente o no se ha avistado nuevos pokemones.\n"ANSI_COLOR_RESET);
                    printf(ANSI_COLOR_YELLOW"/***********************************/\n"ANSI_COLOR_RESET);
                }
                /////////////////////////////////////////////////////////////////////////////////////
            }else if(letra == 'M'){
                /////////////////////////////////////////////////////////////////////////////////////
                printf(ANSI_COLOR_CYAN"Porfavor, escriba el numero de la especie de la que desea informacion:\n"ANSI_COLOR_RESET);
                scanf("%d", &especie);
                pokedex_informacion(pokedex, especie, "");
                printf(ANSI_COLOR_YELLOW"/***********************************/\n"ANSI_COLOR_RESET);
                /////////////////////////////////////////////////////////////////////////////////////
            }else if(letra == 'P'){
                /////////////////////////////////////////////////////////////////////////////////////
                printf(ANSI_COLOR_CYAN"Porfavor, escriba el numero de la especie de la que desea informacion:\n"ANSI_COLOR_RESET);
                scanf("%d", &especie);
                printf(ANSI_COLOR_CYAN"Ahora, escriba el nombre del pokemon del que busca informacion:\n"ANSI_COLOR_RESET);
                scanf("%s", nombre_pokemon);
                pokedex_informacion(pokedex, especie, nombre_pokemon);
                printf(ANSI_COLOR_YELLOW"/***********************************/\n"ANSI_COLOR_RESET);
                /////////////////////////////////////////////////////////////////////////////////////
            }
        }
    }
    return 0;
}