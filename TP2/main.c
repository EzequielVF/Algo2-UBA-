#include "pokedex.h"
#include "lista.h"
#include "abb.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define ERROR -1
#define EXITO 0
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void muestra_opciones_basicas(){
    printf("\t/***MENU***/\n");
    printf("\n\n");
    printf("Iniciar Pokedex (Tecla I)\n");
    printf("Salir del programa (Tecla S)\n");
    printf("Ayuda (Tecla H)\n");
    printf("/*********************************/\n");
}

void muestra_todas_las_opciones(){
    printf("\t/***MENU***/\n");
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
    printf("/*********************************/\n");
}

void mostrar_menu(pokedex_t* pokedex, bool* menu_extendido, bool* quiere_salir){
    char letra = 'a';
    
    while(!(*quiere_salir) && !(*menu_extendido)){
        muestra_opciones_basicas();
        scanf(" %c", &letra);
        while(letra != 'I' && letra != 'S' && letra != 'H'){
            printf("La letra insertada no corresponde a una opcion valida, vuelva a intentarlo.\n");
            scanf(" %c", &letra);
        }
        if(letra == 'H'){
            printf("\n");
        }else if(letra == 'I'){
            pokedex = pokedex_prender();
            *menu_extendido = true;
        }else{
            *quiere_salir = true;
        }
    }
}

void mostrar_menu_desbloqueado(pokedex_t* pokedex, bool* quiere_salir){
    char letra = 'a';
    char nombre_pokemon[MAX_NOMBRE];
    bool letra_valida = false;
    int especie = 0;

    while(!(*quiere_salir)){
        muestra_todas_las_opciones();
        scanf(" %c", &letra);
        if(letra == 'I' || letra == 'G' || letra == 'S' || letra == 'H' || letra == 'A'|| letra == 'E'|| letra == 'C'|| letra == 'V'|| letra == 'M'|| letra == 'P'){
            letra_valida = true;
        }
        while(!letra_valida){
            printf("La letra insertada no corresponde a una opcion valida, vuelva a intentarlo.\n");
            scanf(" %c", &letra);
            if(letra == 'I' || letra == 'G' || letra == 'S' || letra == 'H' || letra == 'A'|| letra == 'E'|| letra == 'C'|| letra == 'V'|| letra == 'M'|| letra == 'P'){
                letra_valida = true;
            }
        }
        if(letra == 'H'){
            printf("\n");
        }else if(letra == 'I'){
            pokedex = pokedex_prender();
        }else if(letra == 'S'){
            *quiere_salir = true;
        }else if(letra == 'G'){
            pokedex_apagar(pokedex);
        }else if(letra == 'A'){
            pokedex_avistar(pokedex, "avistamientos.txt");
        }else if(letra == 'E'){
            pokedex_evolucionar(pokedex, "evoluciones.txt");
        }else if(letra == 'C'){
            pokedex_ultimos_capturados(pokedex);
        }else if(letra == 'V'){
            pokedex_ultimos_vistos(pokedex);
        }else if(letra == 'M'){
            printf("Porfavor, escriba el numero de la especie de la que desea informacion:\n");
            scanf("%d", &especie);
            pokedex_informacion(pokedex, especie, "");
        }else if(letra == 'P'){
            printf("Porfavor, escriba el numero de la especie de la que desea informacion:\n");
            scanf("%d", &especie);
            printf("Ahora, escriba el nombre del pokemon del que busca informacion:\n");
            scanf("%s", nombre_pokemon);
            pokedex_informacion(pokedex, especie, nombre_pokemon);
        }

    }
}

int main(){
    pokedex_t* pokedex = NULL;
    bool menu_extendido = false;
    bool quiere_salir = false;
    
    while(!quiere_salir){
        if(!menu_extendido){
            mostrar_menu(pokedex, &menu_extendido, &quiere_salir);
        }
        if(!quiere_salir && menu_extendido){
            mostrar_menu_desbloqueado(pokedex, &quiere_salir);
        }
    }
    pokedex_destruir(pokedex);
    
    return 0;
}